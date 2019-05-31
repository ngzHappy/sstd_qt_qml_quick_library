import QtQuick 2.9
import QtQuick.Controls 2.5
import QtQuick.Controls.Material 2.3

/*begin:import*/
import theqml_the_debug.sstd.styled.app 1.0
/*end:import*/

ApplicationWindow {

    id : idThis

    /* ------------------------------- */
    function reloadDefaultStyle(){
        GlobalAppData.privateDefaultWindow = idThis;
        GlobalAppData.reloadDefaultStyle();
    }

    /* ------------------------------- */
    function runOnceOnStart(){
        idThis.visible = true;
    }

    /* ------------------------------- */
    Component.onCompleted: {
        /*begin:debug*/
        console.log("Debug : Main Application Window Load !!!");
        /*end:debug*/
        idThis.reloadDefaultStyle();
        Qt.callLater(runOnceOnStart);
    }

}
