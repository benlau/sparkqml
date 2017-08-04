pragma Singleton
import QtQuick 2.0
import SnapshotTesting.Private 1.0

Item {
    id: snapshotTesting

    property string snapshotsFile: Adapter.snapshotsFile

    function capture(object, options) {
        return Adapter.capture(object, options);
    }

    function matchStoredSnapshot(name, snapshot) {
        if (!Adapter.matchStoredSnapshot(name,snapshot)) {
            throw new Error("matchStoredSnapshot: Snapshot does not match with stored snapshot");
        }
    }

    onSnapshotsFileChanged: {
        if (Adapter.snapshotsFile != snapshotTesting.snapshotsFile) {
            Adapter.snapshotsFile = snapshotTesting.snapshotsFile;
        }
    }

}
