import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12
import sstd.styled.app 1.0

ApplicationWindow {
    /* only call after init ... */
    Component.onCompleted : {
        if( GlobalAppData.isDark ){
            Material.theme = Material.Dark  ;
        }else{
            Material.theme = Material.Light ;
        }
    }
}

