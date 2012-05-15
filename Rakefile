
require './src/application'

## Load all the init files present in the subfolders
FileList["*/init.rb"].each do |initfile|
  require "#{initfile}"
end

task :default => [:help]

Application.create_tasks

task :help do	  
  
end


