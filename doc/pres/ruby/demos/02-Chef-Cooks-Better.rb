# Our Goal: Undertand why cookbook_file method is available
# in global scope

require 'CookBook'

class Chef
  include CookBook
end

class << self

  def recipe
    return @recipe unless @recipe.nil?
    @recipe = Chef.new
  end

  ## Callback on general signature missing
  def method_missing(meth_name, *args, &block)
    raise "No method named: #{meth_name}" unless recipe.respond_to? meth_name
    recipe.send(meth_name, *args, &block)
  end

end

cookbook_file "cassandra.conf" do
  # This chunk of code is a block and
  # Will be interpreted in a cookbook context
  source "/cassandra/cassandra.conf.erb"
end

