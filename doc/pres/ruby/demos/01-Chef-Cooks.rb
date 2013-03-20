
# Import the CookBook Module
require 'CookBook'

## Reopening Chef Class
class Chef

  ##Mixing our cookbook
  include CookBook

end

Chef.new.cookbook_file "cassandra.conf" do
  # This chunk of code is a block and
  # Will be interpreted in a cookbook context
  source "/cassandra/cassandra.conf.erb"
end


