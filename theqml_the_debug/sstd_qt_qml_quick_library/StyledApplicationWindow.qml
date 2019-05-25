import QtQuick 2.9
import QtQuick.Controls 2.5
import QtQuick.Controls.Material 2.3
import sstd.styled.app 1.0

ApplicationWindow {

    id : idThis 

    /* ------------------------------- */
    function reloadDefaultStyle(){
        GlobalAppData.privateDefaultWindow = idThis;
        GlobalAppData.reloadDefaultStyle();
    }

    /* ------------------------------- */
    Component.onCompleted: {
        /*begin:debug*/
        console.log("Debug : Main Application Window Load !!!");
        /*end:debug*/
        idThis.reloadDefaultStyle();
    }

}










