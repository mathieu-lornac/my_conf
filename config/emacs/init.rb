
module Emacs
  extend Application

  sys_install({
                :gen => ['emacs'],
                :debian => ['emacs-goodies-el'],
              })
  
  link '~/.emacs', 'emacs/emacs.conf'

end

module Emacs
  module Autocomplete
    extend Emacs
    
    install ["mkdir -p ~/.emacs.d/auto_complete",
             "mkdir /tmp/auto_complete && cd /tmp/autocomplete",
             "wget http://cx4a.org/pub/auto-complete/auto-complete-1.3.1.tar.bz2",
             "tar xf auto-complete-1.3.1.tar.bz2",
             "cd auto-complete-1.3.1 && echo \"~/.emacs.d/auto-complete\" | make install",
             "cd ..",
             "rm -rf auto-complete-1.3.1"
            ]
    
    #   #   conf_routine "~/.emacs", """
    #   #   (add-to-list 'load-path \"~/.emacs.d/auto-complete\")
    #   #   (require 'auto-complete-config)
    #   #   (add-to-list 'ac-dictionary-directories \"~/.emacs.d/auto-complete/ac-dict\")
    #   #   (ac-config-default)
    #   #   """
    
    #  remove  """
    
    # """
  end
end
