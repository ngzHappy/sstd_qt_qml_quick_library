import QtQuick 2.9
import QtQuick.Controls 2.5
import QtQuick.Controls.Material 2.3
import sstd.styled.app 1.0

ApplicationWindow {

    readonly property bool isDark: GlobalAppData.isDark
    function updateTheme(){
        if( isDark ){
            Material.theme = Material.Dark ;
        }else{
            Material.theme = Material.Light ;
        }
    }
    onIsDarkChanged: {
        updateTheme();
    }
    Component.onCompleted: {
        updateTheme();
        visible = true ;
    }

}










