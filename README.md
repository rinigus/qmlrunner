# QML Runner

Runner for QML-only apps. It runs applications assuming that they are installed
in the given prefix `PREFIX` with the following convention

```
PREFIX/app-name/qml/app-name.qml
```

Note that the QML file is loaded assuming that its a window. Don't forget to setup
it to be visible by setting `visible: true` property.

The runner supports translations and fallback icons. For translations, provide `qm` files
in the form

```
PREFIX/app-name/translations/app-name-LOCALE.qm
```

For fallback icons, drop them into

```
PREFIX/app-name/icons
```

To specify `PREFIX`, use `-P` option. Without the option, `/usr/share` is used.

The runner can also look up for QML modules in additional folders. For example,
QML modules installed in the location that is not loaded by default. You could specify
these additional locations through `-path QML_PATH` option. Multiple locations can
be specified by repeating `-path QML_PATH` multiple times, one per location.
