Node.js integration
===================

This module adds Node.js integration to Drupal.

It is currently alpha software, use at your own risk. It may eat your lunch and kill your kittens.

Setup
=====

Node.js server on Ubuntu:

1. Make sure you have the prerequisites in your ubuntu

        sudo apt-get install build-essential git curl openssl libssl-dev -y

2. Install Node.js.

        mkdir -p ~/local/src && cd ~/local/src && git clone git://github.com/joyent/node.git && cd node && ./configure && make && sudo make install

3. Install [npm](http://npmjs.org/) and run `npm install` (our server relies on
   [Socket.io](http://socket.io/), [connect](http://senchalabs.github.com/connect/),
   and [express](http://expressjs.com/)).

        sudo curl http://npmjs.org/install.sh | sh
        cd /into/the/nodejs/drupalmodule
        npm install

4. Optionally, copy 'server.js' to a directory specific to the Node.js server used by Drupal.

5. Activate the nodejs module and optionally sub-modules.

6. Optionally, copy 'server.js' to a directory specific to the Node.js server used by Drupal.

7. Configure nodejs at your Drupal page: Administration > Configuration > Node.js

8. If you enabled the nodejs_config module, set the fields and save it.
   or copy 'nodejs.config.js.example' to 'nodejs.config.js' (in the same
   directory where server.js is located). Edit the values to taste.

9. Run the node server with the command: node server.js

11. ...

12. Profit!

Useful links:

  - how to turn nodejs into a service http://kevin.vanzonneveld.net/techblog/article/run_nodejs_as_a_service_on_ubuntu_karmic/

Troubleshooting:

  - If running node server.js returns an error:
    - Check your versions of Express and Connect. This module works with Connect 1.1.4 and Express 2.4.3,
      which are NOT the versions npm (node package manager) installs by default.
      You may need to roll these back, which can be done with npm, using these commands:
        - npm install connect@1.1.4
        - npm install express@2.4.3
