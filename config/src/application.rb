require './src/distrib'

module Link
  @@links = {}

  def link (dst, src)
    @@links[self] = {} unless @@links[self]
    @@links[self][dst.to_sym] = src
  end

  def self.action(name, application)
    puts "Unrecognized action: #{action}" if name != :install and name != :remove 
    @@links[application].each {|k, v|
      puts "ln -s #{v} #{k}"
    }
  end
end

module Application

  include Link

  @@apps = {}
  @@features = {}
  @@sys_install = {}
  @@install = {}

  def self.features
    @@features
  end

 def self.install
    @@install
  end

  def name
    "#{self.to_s.downcase}".to_sym
  end

  def self.extended(other)
    @@apps[other.to_s.downcase.to_sym] = other 
    other.class_eval <<-RUBY
    def self.extended(other)
      Application.features[self] = [] unless Application.features[self]
      Application.features[self] << other
      other.extend Application
    end
    RUBY
  end
  
  def self.sys(action, name)
    @@sys_install[name][:gen].each do |p|
      puts "#{Distrib::pacman(action.to_sym)} #{p}"
    end if @@sys_install[name][:gen]
    @@sys_install[name][Distrib::name].each do |p|
      puts "#{Distrib::pacman(action.to_sym)} #{p}"
    end if @@sys_install[name][Distrib::name]
  end

  ### The job could be done using method_missing?
  def sys_install(map)
    @@sys_install[self.name] = map
  end

  def install(commands)
    @@install[self.name] = commands
  end

  # Creates the nested tasks to deploy an application feature
  def create_tasks
    Application.features[self].each{ |f|
      extend Rake::DSL
      taskname = "#{f.to_s.split('::').last}"
      desc "Feature"
      task taskname => ["#{taskname}:install"] do
      end
      namespace taskname do
        desc "install #{taskname}"
        task :install  do
          puts "----> installing #{taskname}"
          puts "#{self} | #{f}"
          Application.install[f.name].each{ |c|
            puts "#{c}"
          }
        end
      end 
    } if Application.features[self]
  end

  # Creates the root tasks to deploy an application
  def self.create_tasks
    @@apps.each do |k, app| next if k.to_s.include?('::')
      extend Rake::DSL
      taskname = "#{app.to_s.split('::').last.downcase}"
      desc "Install the application #{taskname} with all it's features"
      task "#{app.to_s.split('::').last.downcase}" => ["#{taskname}:sys_install", "#{taskname}:install", "#{taskname}:link"] do       
      end
      namespace taskname do
        desc "install #{taskname}"
        task :install do
           puts "===> installing #{taskname}"
          Application.install[app.name].each{ |c|
            puts "#{c}"
          }
        end

        desc "Systems packages install #{taskname}"
        task :sys_install  do
           puts "===> installing system deps for #{taskname}"
          Application.sys(:install, taskname.to_sym)
        end

        desc "link #{taskname} => creates all the symbolic links to point to this config"
        task :link  do
           puts "===> linking #{taskname}"
          Link.action(:install, app)
        end

        app.create_tasks
      end
    end
  end 
end
