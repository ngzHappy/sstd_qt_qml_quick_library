
import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12

/*begin:import*/
import theqml_the_debug.sstd.styled.app 1.0
/*end:import*/

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

