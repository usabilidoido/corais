/**
 * backendSettings defines configuration settings for server.js, the server code
 * executed by node.
 *
 * The following properties should be defined for the backendSettings variable:
 *
 * scheme: 'http' or 'https'. This defines whether the node server should
 * contact the Drupal site using http or https. If https is used, the key and
 * cert must be set to valid files.
 *
 * host: Specify the host name or IP address that the node server should listen
 * on. Leave blank to listen for any host name. Otherwise, the server will only
 * respond to names that match the IP address given (or resolved from the given
 * name).
 *
 * port: Specify the TCP port that the node server should listen on.
 *
 * key: File system path to a key used for https communication with the server
 * and clients.
 *
 * cert: File system path to a certificate used for https communication with the
 * server and clients.
 *
 * resource: http path that the node server should respond to. This value needs
 * to match the Drupal node.js configuration.
 *
 * publishUrl: http path on which the node server should accept messages from
 * the Drupal site.
 *
 * serviceKey: An arbitrary string used as a secret between the node.js server
 * and the Drupal site.
 *
 * backend: An object with the following properties:
 *
 *  -- port: TCP port of the server running the Drupal site. Usually 80.
 *
 *  -- host: Host name of the Drupal site.
 *
 *  -- authPath: http path on which the Drupal node.js module listens for
 *  authentication check requests. Must end with /.
 **/

backendSettings = {
  'scheme': 'http',
  'host': 'localhost',
  'port': 8080,
  'key': '/path/to/key/file',
  'cert': '/path/to/cert/file',
  'resource': 'socket.io',
  'publishUrl': '/nodejs/publish',
  'serviceKey': '',
  'backend': {
    'port': 80,
    'host': 'drupal6',
    'messagePath': '/nodejs/message'
  },
  'extensions': [],
  'clientsCanWriteToChannels': false,
  'clientsCanWriteToClients': false,
  'transports': ['websocket', 'flashsocket', 'htmlfile', 'xhr-polling', 'jsonp-polling'],
  'jsMinification': true,
  'jsEtag': true,
  'logLevel': 1
}; 