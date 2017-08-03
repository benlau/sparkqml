#ifndef SNAPSHOTTESTINGOPTIONS_H
#define SNAPSHOTTESTINGOPTIONS_H


namespace SnapshotTesting {

    class Options {
    public:

        inline Options() {
            captureVisibleItemOnly = true;
            expandAll = false;
        }

        bool captureVisibleItemOnly;
        bool expandAll;
    };

}

#endif // SNAPSHOTTESTINGOPTIONS_H
