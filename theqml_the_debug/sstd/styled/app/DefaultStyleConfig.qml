import QtQuick 2.9
import QtQuick.Controls 2.5
import QtQuick.Controls.Material 2.3

/*begin:import*/
import theqml_the_debug.sstd.styled.app 1.0
/*end:import*/

QtObject{

    function getDefaultIsDark(){
        return false;
    }

    function reloadDefaultStyle(){
        GlobalAppData.isDark = getDefaultIsDark();
    }

}
