if get_option('gtk_doc') and not get_option('introspection').allowed()
  error('API reference requires introspection.')
endif

toml_conf = configuration_data()
toml_conf.set('version', meson.project_version())

gidocgen = find_program('gi-docgen', required: get_option('gtk_doc'))

gidocgen_common_args = [
  '--quiet',
  '--no-namespace-dir',
]

if get_option('werror')
  gidocgen_common_args += ['--fatal-warnings']
endif

docs_dir = datadir / 'doc'

if get_option('gtk_doc')
  source_toml = configure_file(
    input: 'GUdev.toml.in',
    output: 'GUdev.toml',
    configuration: toml_conf,
    install: true,
    install_dir: docs_dir / package_name,
  )

  custom_target('gudev-doc',
    input: [ source_toml, libgudev_gir[0] ],
    output: package_name,
    command: [
      gidocgen,
      'generate',
      gidocgen_common_args,
      '--config=@INPUT0@',
      '--output-dir=@OUTPUT@',
      '--content-dir=@0@'.format(meson.current_source_dir()),
      '@INPUT1@',
    ],
    build_by_default: true,
    install: true,
    install_dir: docs_dir,
  )
endif
