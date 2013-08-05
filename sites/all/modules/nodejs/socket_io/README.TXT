Socket.IO Setup
================

Nodejs.module requires Socket.IO to function.

Two options for installation:

1. Install npm (Node Package Manager) 
curl http://npmjs.org/install.sh | sudo sh

2. Use npm to install Socket.IO and Express, and symlink into this folder.

> npm install socket.io express
> ln -s /path/to/socket/io/install/socket.io /path/to/your/nodejs/module/socket_io/socket.io

if you installed node.js in ~/local/src, the symlink will be
> ln -s /usr/local/lib/node/socket.io/ socket_io

OR

3. Download from Socket.IO from http://socket.io into this folder.

