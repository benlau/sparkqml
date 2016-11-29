import QtQuick 2.7

import "./components"
import "./browserpanel"
import "./mainpanel"

PageStack {
    id: component
    clip: true

    views: provider.views

    pushEnter: Transition {
        PropertyAction {
            property: "opacity"
            value: 0
        }

        OpacityAnimator {
            from: 0
            to: 1
            duration: 200
            easing.type: Easing.Linear
        }
    }

    pushExit: Transition {
        PauseAnimation {
            duration: 200
        }
    }

    popEnter: Transition {
    }

    popExit: Transition {
        OpacityAnimator {
            from: 1
            to: 0
            duration: 200
            easing.type: Easing.Linear
        }
    }

    model: Item {
        property Component browserPanel: BrowserPanel {
        }
    }

    initItem: MainPanel {
    }

}
