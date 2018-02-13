{
  "targets": [
    {
      "target_name": "addon",
      "include_dirs": ['RIOT/sys', 'RIOT/sys/include'],
      "sources": [ "node_bridge.cpp", "eid.h", "eid.c"],
      "defines": [ "SERVER" ]
    }
  ]
}
