
.import sstd.styled.app 1.0 as JSGlobalApp

/******************************************/

function getDefaultIsDark(){
    return false;
}

/******************************************/

function reloadDefaultStyle() {

    JSGlobalApp.GlobalAppData.isDark = getDefaultIsDark();

}




