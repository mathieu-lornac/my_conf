## Chef.new optionnal
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
