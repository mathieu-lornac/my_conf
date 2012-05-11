require './src/distrib'

module Link
  def link (dst, src)
  end
end

module Application
end


module Application
  @@apps = {}
  @@features = {}

  def self.features
    @@features
  end

  include Link

  def name
    "#{self.to_s.downcase}".to_sym
  end

  @@sys_install = {}

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

  def create_tasks
    puts Application.features
    puts self
    Application.features[self].each{ |f|
      extend Rake::DSL
      taskname = "#{f.to_s.split('::').last}"
      desc "Feature"
      task taskname  do
      end
      namespace taskname do
        desc "install #{taskname}"
        task :install  do
          puts "----> installing #{taskname}"
          
        end
      end
    }
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

  def self.create_tasks
    @@apps.each do |k, app| next if k.to_s.include?('::')
      extend Rake::DSL
      taskname = "#{app.to_s.split('::').last.downcase}"
      desc "Application: #{taskname} | install #{taskname} with all it's features"
      task "#{app.to_s.split('::').last.downcase}" => ["#{taskname}:sys_install"] do       
      end
      namespace taskname do
        desc "install #{taskname}"
        task :install  do
           puts "===> installing #{taskname}"

        end
        desc "Systems packages install #{taskname}"
        task :sys_install  do
           puts "===> installing system deps for #{taskname}"
          Application.sys(:install, taskname.to_sym)
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
  
    #   install """
    #   #   mkdir -p ~/.emacs.d/auto_complete
    #   #   mkdir /tmp/auto_complete && cd /tmp/autocomplete
    #   #   wget http://cx4a.org/pub/auto-complete/auto-complete-1.3.1.tar.bz2
    #   #   tar xf auto-complete-1.3.1.tar.bz2
    #   #   cd auto-complete-1.3.1 && echo \"~/.emacs.d/auto-complete\" | make install
    #   #   cd ..
    #   #   rm -rf auto-complete-1.3.1
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
