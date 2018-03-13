libgudev_tests = {
  'test-enumerator-filter': {
    'dependencies': [gio_dep, umockdev_dep],
    'environment': ['LD_PRELOAD=libumockdev-preload.so.0'],
  },
  'test-sysfsattr': {
    'dependencies': [gio_dep, umockdev_dep],
    'environment': ['LD_PRELOAD=libumockdev-preload.so.0'],
  },
}

libgudev_tests_deps = [
  libgudev_dep,
]

foreach test_name, test_extras: libgudev_tests
  test_sources = ['@0@.c'.format(test_name)]
  test_deps = libgudev_tests_deps
  test_env = []

  test_deps += test_extras.get('dependencies', [])
  test_env += test_extras.get('environment', [])

  test_exe = executable(
    test_name,
    test_sources,
    dependencies: test_deps,
  )

  test(
    test_name,
    test_exe,
    env: test_env,
  )
endforeach
