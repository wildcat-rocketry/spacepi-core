# Deploy Key Setup Utility

This program's purpose is to associate a user's SSH key as a GitHub account's SSH key.
Normally, this process would be taken care of by manually adding it in [settings](https://github.com/settings/keys), however this utility makes it easier.
This program handles all of the communications with GitHub, figuring out the key to use, etc. to allow for quicker setup after imaging a new Pi.
Having the SSH key set in GitHub helps because it allows pushing commits directly from the Pi, so debugging changes can be committed easier.

A simple use of this program can be seen below:

```
myuser@my-spacepi-instance:~$ setup-deploy-key
[2020-01-01 00:00:00] [INFO ] [core:messaging] Network thread starting...
[2020-01-01 00:00:01] [INFO ] [os:setup-deploy-key] Ready to connect to GitHub.
[2020-01-01 00:00:01] [INFO ] [os:setup-deploy-key] Please navigate to one of the following addresses in your browser to complete setup:
[2020-01-01 00:00:01] [INFO ] [os:setup-deploy-key]   http://10.1.1.42:8000/
[2020-01-01 00:00:01] [INFO ] [os:setup-deploy-key]   http://10.42.0.73:8000/
[2020-01-01 00:02:00] [INFO ] [os:setup-deploy-key] Got authorization from GitHub.
[2020-01-01 00:02:01] [INFO ] [os:setup-deploy-key] Added the following key(s) to myusername@github.com:
[2020-01-01 00:02:01] [INFO ] [os:setup-deploy-key]   01:23:45:67:89:ab:cd:ef:fe:dc:ba:98:76:54:32:10 (/home/myuser/.ssh/id_rsa.pub)
[2020-01-01 00:02:02] [INFO ] [core:messaging] Networking thread shutting down.
```

Note: Since all of the users by default have the same SSH keys, this will add the keys for all user accounts.

## Technical Details

Here is a more technical list of steps which happen when the program is run:

1. Open an HTTP server on port `8000`, listening on all interfaces (`0.0.0.0`)
2. Figure out a list of current IP addresses (and ignore `127.0.0.0/8`) and print all possible web links to access the server
3. Redirect any requests to the web server (except the request mentioned in step 4) to `https://ffaero.com/static/github-auth?target=10.1.1.42%3A8000%2Fcallback&scope=write%3Apublic_key&allow_signup=false` (where `10.1.1.42%3A8000%2Fcallback` represents the encoded URL for step 4, so its IP should come from the `Host` HTTP request field)
4. When a request for `/callback?code=XXXXXXXXXX` is received, shut down the web server and use that code for future GitHub API calls
5. Search for public keys (files in `$HOME/.ssh` which end in `.pub`) and add them to the GitHub account if they do not already exist
6. Print out which keys were added then exit the program

### Technical References

* Networking library with Boost: [Boost.Asio](https://www.boost.org/doc/libs/1_74_0/doc/html/boost_asio.html)
* HTTP library with Boost: [Boost.Beast](https://www.boost.org/doc/libs/1_74_0/libs/beast/doc/html/index.html)
* JSON parser library with Boost: [Boost.PropertyTree](https://www.boost.org/doc/libs/1_74_0/doc/html/property_tree.html)
* Getting IP addresses on Linux: [getifaddrs(3)](https://www.man7.org/linux/man-pages/man3/getifaddrs.3.html)
* Getting files in a folder on Linux: [readdir(3)](https://www.man7.org/linux/man-pages/man3/readdir.3.html)
* GitHub authorization using a token: [GitHub Authentication Docs](https://docs.github.com/en/free-pro-team@latest/rest/overview/resources-in-the-rest-api#authentication)
* Uploading SSH keys: [GitHub SSH Key API](https://docs.github.com/en/free-pro-team@latest/rest/reference/users#git-ssh-keys)
