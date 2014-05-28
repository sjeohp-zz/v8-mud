
{
  "targets" : [
  {
    "target_name": "mud",
    "sources": ["mud.cc", "src/commands.cc", "src/player.cc", "src/sockets.cc", "src/trie.cc", "src/scrypt.cc"],
    "include_dirs": ["frameworks"],
    "conditions": [
      [ 'OS=="mac"', {
        "xcode_settings": {
          'OTHER_CPLUSPLUSFLAGS' : ['-std=c++11','-stdlib=libc++'],
          'OTHER_LDFLAGS': ['-stdlib=libc++'],
          'MACOSX_DEPLOYMENT_TARGET': '10.9'
        }
      }],
      [ 'OS=="linux"', {
        "cflags": ["-std=c++11"]
      }]
    ]
  }]
}
