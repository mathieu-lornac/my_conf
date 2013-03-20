

module CookBook

  ## Keep record of files to cook
  @@files_to_cook = []

  def cookbook_file(name, &block)
    f = PCookBook.new name
    @@files_to_cook << f
    # Evaluation of the block in the PClass context
    f.instance_eval &block
  end

private
  class PCookBook
    attr_accessor :name

    def initialize(name)
      @name = name
    end

    def source(path)
      puts "CB filepath: #{path}"
    end

  end
end
