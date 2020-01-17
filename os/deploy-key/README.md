# Space Pi Deploy Keys

This directory contains a SSH key that is shared between all users on the Space Pi system.
This key is used for pushing to the Git repo after making changes to the code directly on the Pi.

## Git Integration

The key is ignored from Git so you cannot commit it (as you should never share the private key with anyone).
Also, it can be added as a key specifically to the project by going to the [Repository Settings](https://gitlab.com/ffaero/tools/space-pi/-/settings/repository) and selecting "Deploy Keys."
Make sure to check the "Write access allowed" box so you can push to the repo.
