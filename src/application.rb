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
      puts ENV['PWD']
      }
  end
end

module Application
end


module Application

  include Link

  @@apps = {}
  @@features = {}
  @@sys_install = {}

  def self.features
    @@features
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
    end
    @@sys_install[name][Distrib::name].each do |p|
      puts "#{Distrib::pacman(action.to_sym)} #{p}"
    end
  end

  def sys_install(map)
    @@sys_install[self.name] = map
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
          
        end
      end
    }
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

module Emacs
  extend Application

  sys_install({:gen => ['emacs'],
                :debian => ['emacs-goodies-el'],
              })
  
  link '~/.emacs', 'emacs/emacs.conf'

  module Autocomplete
    extend Emacs
  
     # install "Autocomplete install procedure"

# """
    #     mkdir -p ~/.emacs.d/auto_complete
    #     mkdir /tmp/auto_complete && cd /tmp/autocomplete
    #     wget http://cx4a.org/pub/auto-complete/auto-complete-1.3.1.tar.bz2
    #     tar xf auto-complete-1.3.1.tar.bz2
    #     cd auto-complete-1.3.1 && echo \"~/.emacs.d/auto-complete\" | make install
    #     cd ..
    #     rm -rf auto-complete-1.3.1
    # """
    
    #   #   conf_routine "~/.emacs", """
    #   #   (add-to-list 'load-path \"~/.emacs.d/auto-complete\")
    #   #   (require 'auto-complete-config)
    #   #   (add-to-list 'ac-dictionary-directories \"~/.emacs.d/auto-complete/ac-dict\")
    #   #   (ac-config-default)
    #   #   """

    #   remove  """
    
    #   """
  end
end


    # module Multimode
    #   extend Emacs::Feature
      
    # end

  #   module Zsh
  #     extend Application
    
  #     _sys_install({ :gen => ['zsh'] })

  #     install_routine "~/.oh-my-zsh", "git clone git://github.com/robbyrussell/oh-my-zsh.git ~/.oh-my-zsh"
      
  #     remove_routine """
  #     rm -rf ~/.oh-my-zsh
  #     rm -f ~/.zshrc
  #     """
  #     link '~/.zshrc', 'zsh/zshrc.conf'

  #   end
  # end



=begin

module Emacs
  extend Application
  sys_install({:gen => ['emacs'],
                :debian => ['emacs-goodies-el']
              })

  install """
  install procedure
  """
  remove
  
  link "~/.emacs" "emacs.conf"
    
end

Doit generer les targets:

- emacs => emacs:install

- emacs:install => emacs:install:#{module}:install
 - emacs:install:all
- emacs:remove

=end
