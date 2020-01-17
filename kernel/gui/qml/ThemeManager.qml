import QtQuick 2.0

Item {
    id: themeManager

    // Color for Layers
    property string background: "#323232"
    property string midground: "#eaeaea"
    property string foreground: "#ffffff"

    // Color for Highlights
    property string blue: "#2a3f54"
    property string blueHighlight: "#354a5f"

    // Coler & Size for Fonts
    property string fontColer: "#ffffff"
    property string backColer: "#000000"
    property int fontSizeDefault: 12
    property int fontSizeMiddle: 14
    property int fontSizeBig: 16
    property int fontSizeExtraBig: 24
    property int fontSizeExtraExtraBig: 30
    property bool isBold: false

    // Default colors
    property string black: "black"
    property string white: "white"
    property string green: "#1ed760"

    property int navigationMenuHeight: 50
    property int groupMenuHeight: 50
    property int navigationMenuMargin: 10
    property int navigationTopMenuHeight: 75
}
