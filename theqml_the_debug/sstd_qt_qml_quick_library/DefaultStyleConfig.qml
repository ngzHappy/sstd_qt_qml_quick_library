﻿import QtQuick 2.9
import QtQuick.Controls 2.5
import QtQuick.Controls.Material 2.3
import sstd.styled.app 1.0


QtObject{

    function getDefaultIsDark(){
        return false;
    }

    function reloadDefaultStyle(){
        GlobalAppData.isDark = getDefaultIsDark();
    }

}
