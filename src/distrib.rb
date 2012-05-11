#
#Abstracts distribution dependancy calls
#
module Distrib
  
  @@distribs= {}

  #returns the package manager of the running distribution
  def self.pacman(command = nil)
    dist = `lsb_release -i`
    d = @@distribs.keys.detect {|k| dist.include? k.to_s.split('::').last}
    puts "The distribution you are running (#{dist}) is not handled" if d.nil?
    return @@distribs[d].pacman(command)
  end
  
  def self.name
    dist = `lsb_release -i`
    d = @@distribs.keys.detect {|k| dist.include? k.to_s.split('::').last}
    return @@distribs[d].to_s.split('::').last.downcase.to_sym if d
  end

  def self.extended(other)
    @@distribs[other.to_s] = other
  end

  private

  def package_manager(manager_name, *option_map)
    puts option_map
    class_eval <<-RUBY 
    def self.pacman(command = nil)
      return "#{manager_name}" if command.nil?
      return "#{manager_name} \#{@@pacman_commands[command]}"
    end
    @@pacman_commands = {}
   RUBY
    option_map.first.each_pair {|k, v|
      class_eval "@@pacman_commands[:#{k}] = '#{v}'"
    }
  end 
end


module Distrib
  module Debian
    extend Distrib
  
    package_manager 'apt-get', {
      :install => 'install',
      :remove => 'remove',
    } 
  end

  module Archlinux
    extend Distrib
  
    package_manager 'pacman', {
      :install => '-S'
    } 
  end

end
