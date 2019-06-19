# windows:
	# Copies the contents of the id_rsa.pub file to your clipboard
	clip < ~/.ssh/id_rsa.pub

# linux:
	# Downloads and installs xclip. If you don't have `apt-get`, you might need to use another installer (like `yum`)
    sudo apt-get install xclip
    # Copies the contents of the id_rsa.pub file to your clipboard
	xclip -sel clip < ~/.ssh/id_rsa.pub
	