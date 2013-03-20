
module Zsh
  extend Application
  
  sys_install({
                :gen => ['zsh']
              })
  
  link '~/.zshrc', 'zsh/zshrc.conf'

  install ["git clone git://github.com/robbyrussell/oh-my-zsh.git ~/.oh-my-zsh"]
  
  # remove [
  #         "rm -rf ~/.oh-my-zsh",
  #         "rm -f ~/.zshrc",
  #        ]

end
