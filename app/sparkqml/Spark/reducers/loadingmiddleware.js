// Before actually load a source, scan import path file first
.import "../actions/actiontypes.js" as ActionTypes
.import Future 1.0 as FuturePkg
.import QUIKit 1.0 as QUIKitPkg
.import Spark.sys 1.0 as SparkSys

var Future = FuturePkg.Future;
var Shell = QUIKitPkg.Shell;
var Engine = SparkSys.Engine;

function create(settings) {
    return function (store) {

        return function(next) {

            return function (action) {

                if (action.type === ActionTypes.load) {
                    Engine.clearComponentCache();
                    Engine.errorString = "";
                    var future = Engine.scanImportPathList(action.source);
                    Future.onFinished(future, function() {
                        next(action);
                    });
                    return;
                }
                next(action);
            }
        }
    }
}
