# QML Runner

Runner for QML-only apps. It runs applications assuming that they are installed
in the given prefix (`/usr/share`) by default with the following convention

```
PREFIX/app-name/qml/app-name.qml
```

Note that the QML file is loaded assuming that its a window. Don't forget to setup
it to be visible by setting `visible: true` property.
