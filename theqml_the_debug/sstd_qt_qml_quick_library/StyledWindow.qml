import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12
import sstd.styled.app 1.0

ApplicationWindow {
    readonly property bool isDark: GlobalAppData.isDark
    function updateTheme(){
        if( isDark ){
            Material.theme = Material.Dark  ;
        }else{
            Material.theme = Material.Light ;
        }
    }
    onIsDarkChanged: {
        updateTheme();
    }
    Component.onCompleted: {
        updateTheme();
    }
}






