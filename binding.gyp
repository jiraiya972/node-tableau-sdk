{
'make_global_settings': [
    ['CXX','/usr/bin/clang++'],
    ['LINK','/usr/bin/clang++'],
  ],
  "targets": [
    {
      "target_name": "tableau",
      "sources": [
        "src/node-tde.cc",
        "src/TableauExtract.cc",
        "src/TableauTable.cc",
        "src/TableauTableDefinition.cc",
        "src/TableauRow.cc",
        "src/TableauServerConnection.cc"
      ],
      "cflags": [
        "-std=c++11",
        "-stdlib=libc++"
      ],
      "cflags!": [
        "-fno-exceptions"
      ],
      "cflags_cc!": [
        "-fno-exceptions"
      ],
      "conditions": [
        [
          "OS==\"linux\"",
          {
            "cflags+": [
              "-std=c++11",
              "-stdlib=libc++"
            ],
            "cflags_c+": [
              "-std=c++11",
              "-stdlib=libc++"
            ],
            "cflags_cc+": [
              "-I../lib/tableausdk/include",
              "-std=c++0x"
            ],
            "link_settings": {
              "libraries": [
                "-lTableauCommon",
                "-lTableauExtract",
                "-lTableauServer"
              ],
              "ldflags": [
                "-L../lib/tableausdk/lib64/tableausdk",
                "-Wl,-z,defs,-rpath,'$$ORIGIN/tableausdk/lib64/tableausdk'"
              ]
            }
          }
        ],
        [
          "OS==\"mac\"",
          {
            "xcode_settings": {
              "OTHER_CPLUSPLUSFLAGS": [
                "-std=c++11",
                "-stdlib=libc++"
              ],
              "OTHER_LDFLAGS": [
                "-stdlib=libc++",
                "-framework TableauCommon",
                "-framework TableauExtract",
                "-framework TableauServer"
              ],
              "MACOSX_DEPLOYMENT_TARGET": "10.7",
              "GCC_ENABLE_CPP_EXCEPTIONS": "YES"
            }
          }
        ]
      ]
    },
    {
      "target_name": "action_after_build",
      "type": "none",
      "dependencies": [ "<(module_name)" ],
      "copies": [
        {
          "files": [ "<(PRODUCT_DIR)/<(module_name).node" ],
          "destination": "<(module_path)"
        }
      ]
    }
  ]
}
