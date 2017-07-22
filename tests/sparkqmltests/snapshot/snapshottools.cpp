#include <QDebug>
#include <QStringList>
#include "snapshottools.h"

using namespace std;
#include <iostream>
#include <sstream>
#include <vector>
#include "snapshot/dtl/Sequence.hpp"
#include "snapshot/dtl/Lcs.hpp"
#include "snapshot/dtl/variables.hpp"
#include "snapshot/dtl/functors.hpp"
#include "snapshot/dtl/Ses.hpp"
#include "snapshot/dtl/Diff.hpp"

QString SnapshotTools::diff(QString original, QString current)
{
    auto toVector = [=](QString text) {
        vector<string> res;

        QStringList lines = text.split("\n");
        for (int i = 0 ; i < lines.size() ;i++) {
            res.push_back(lines[i].toStdString());
        }

        return res;
    };

    vector<string> text1, text2;

    text1 = toVector(original);
    text2 = toVector(current);
    dtl::Diff<string> diff(text1, text2);

    diff.onHuge();
    diff.compose();
    diff.composeUnifiedHunks();

    std::stringstream stream;

    diff.printUnifiedFormat(stream);

    return QString::fromStdString(stream.str());
}
