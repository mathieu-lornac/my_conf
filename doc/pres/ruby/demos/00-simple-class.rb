
class A

  ##
  ## Default Ctor
  ##
  def initialize()
    @@static_var = []
    @instance_map = {:hello => 'World'}
    puts 'In A::Ctor'
  end

  attr_accessor :instance_map

  ##
  ## Pretty basic instance method
  ##
  def instance_method()
    puts "This method accepts: " +
      "#{self.method(:instance_method).arity}  arguments"
  end

  ##
  ## Pretty basic Class method
  ##
  def self.class_method()
    puts "We defined: #{self.instance_methods(false).size} methods"
  end

end


