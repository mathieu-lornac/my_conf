#
#Abstracts distribution dependancy calls
#
module Distrib
  
  @@distribs= {}

  #returns the package manager of the running distribution
  def self.pacman
    dist = `lsb_release -i`
    d = @@distribs.keys.detect {|k| dist.include? k.to_s.split('::').last}
    puts "The distribution you are running (#{dist}) is not handled" if d.nil?
    return @@distribs[d].pacman
  end
  
  def self.extended(other)
    @@distribs[other.to_s] = other
  end

  private

  def package_manager(manager_name)
    puts "In package manager"
    puts self
    puts "*" * 80
    class_eval <<-RUBY 
    def self.pacman
      return "#{manager_name}"
    end
   RUBY
  end 
end


module Distrib
  module Debian
    extend Distrib
  
    package_manager 'apt-get'
  end
end
