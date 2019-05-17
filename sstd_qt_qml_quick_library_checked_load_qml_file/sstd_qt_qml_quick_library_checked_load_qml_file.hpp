namespace sstd{

    namespace _private_sstd_checked_load_qml_file{

        class ConnectData {
        public:
            QCoreApplication * app;
            QQmlApplicationEngine * engine;
            QUrl url;
            QMetaObject::Connection connectIndex;
        private:
            sstd_class(ConnectData);
        };

    }/*namespace _private_sstd_checked_load_qml_file*/

    inline void checkedLoadQmlFile(QQmlApplicationEngine * argEngine,
                                   QCoreApplication * argApp,
                                   const QUrl & argUrl){
        auto varConnectData = sstd_make_shared< _private_sstd_checked_load_qml_file::ConnectData >(
            argApp,
            argEngine,
            argUrl);
        varConnectData ->connectIndex =
                argEngine->connect( argEngine,&QQmlApplicationEngine::objectCreated,
                                    argApp , [varConnectData](QObject *argAnsObj, const QUrl &argAnsUrl){
                if(argAnsUrl == varConnectData->url){
                varConnectData->engine->disconnect(varConnectData->connectIndex);
    }else{return;}
                if( !argAnsObj ){
                varConnectData->app->exit(-1);
    }
    });
        argEngine->load(argUrl);
    }

}/*namespace sstd*/

