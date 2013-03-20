## Our Goal:
## Display a message everytime a cookbook_file call is performed

require 'CookBook'
require 'common.rb'

class Chef
  include CookBook
end

# Dummy Log class
class Log
  def self.instance
    return @log unless @log.nil?
    @log = Log.new
  end

  def <<(to_log)
    puts "Logging: " << to_log
  end
end

# cookbook_file is mixed in Chef class
# We reach it from here
class Chef
  # We keep a handle on the current method implementation
  @@chef_orig_cookbook_file = Chef.instance_method :cookbook_file

  # We define a new cookbook_file method
  def cookbook_file(*args, &block)
    Log.instance << "About to cook the file: #{args[0]}"
    @@chef_orig_cookbook_file.bind(self).call(args, &block)
    Log.instance << "Hoora the file: #{args[0]} has been cooked"
  end
end

cookbook_file "I_cook_but_I_log.conf" do
   source "/cassandra/cassandra.conf.erb"
end
