require './src/distrib'

module Application

  @@apps = {}

  def self.extended(other)
    @@apps[other.to_s.split('::').last.downcase.to_sym] = other
  end
  
  def self.install(name)
    app = @@apps[name.to_sym]
    puts "#{name}: No such application registered"if app.nil?
    puts "Installing #{name} base package(s) on the system"
    app.sys_install
  end

  def self.remove(name)
  end

private
  def _sys_install(map)
    class_eval "@@install_sys = {};"
    map.each_pair do |k, v|
      class_eval "@@install_sys[:#{k}] = []"
      v.each {|val| puts "EVAL: #{k} | #{val}"; class_eval "@@install_sys[:#{k}] << '#{val}'"}
    end
    class_eval <<-RUBY
    def self.sys_install
      # General packages
      @@install_sys[:gen].each do |p|
        puts "#{Distrib::pacman(:install)} \#{p}"
      end
      # Distrib specific packages
      @@install_sys[:#{Distrib::name}].each do |p|
        puts "#{Distrib::pacman(:install)} \#{p}"
      end
    end
    RUBY
  end

  module Feature
    def remove_routine(extra_instructions)
    end

    def install_routine (dest_folders, routine)
      @@install_folders = dest_folders
    end

    def conf_routine (file, conf)
    end
  end
end

module Application
  module Emacs
    extend Application
    
    _sys_install({:gen => ['emacs'],
      :debian => ['emacs-goodies-el']
    })
    
    module Autocomplete
      extend Application::Feature
      
      install_routine "~/.emacs.d/auto-complete", """
      mkdir -p ~/.emacs.d/auto_complete
      mkdir /tmp/auto_complete && cd /tmp/autocomplete
      wget http://cx4a.org/pub/auto-complete/auto-complete-1.3.1.tar.bz2
      tar xf auto-complete-1.3.1.tar.bz2
      cd auto-complete-1.3.1 && echo \"~/.emacs.d/auto-complete\" | make install
      cd ..
      rm -rf auto-complete-1.3.1
      """
      
      conf_routine "~/.emacs", """
      (add-to-list 'load-path \"~/.emacs.d/auto-complete\")
      (require 'auto-complete-config)
      (add-to-list 'ac-dictionary-directories \"~/.emacs.d/auto-complete/ac-dict\")
      (ac-config-default)
      """

      remove_routine """
     
      """
    end
    
  end
end
