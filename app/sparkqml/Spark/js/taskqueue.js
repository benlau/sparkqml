.pragma library
.import "./lodash/lodashwrapper.js" as Lodash
.import QuickPromise 1.0 as QPPkg

var QPTimer = QPPkg.QPTimer;

function TaskQueue() {
    this.queue = [];
    this.pending = false;
}

TaskQueue.prototype.enqueue = function(weight, callback) {
    var task = {
        weight: weight,
        callback: callback
    }

    this.queue.push({weight: weight, callback: callback});

    if (this.pending) {
        return;
    }

    this.pending = true;
    var thiz = this;
    QPTimer.setTimeout(this.dequeue.bind(this), 0);
}

function dequeue(TaskQueue) {
    return function() {
        TaskQueue.dequeue();
    }
}

TaskQueue.prototype.dequeue = function() {
    var promise;

    function compare(a,b) {
        return a.weight - b.weight;
    }

    while (promise === undefined && this.queue.length > 0) {
        this.queue = this.queue.slice(0); // QTBUG-57545

        // Sort the array for each iteration. Because new record may be added in callback function
        this.queue.sort(compare);
        var next = this.queue.shift();
        try {
            console.log("Run", next.weight);
            promise = next.callback();
            if (promise) {
                promise.then(dequeue(this));
            }
        } catch(e) {
            console.log(e);
        }
    }

    if (promise === undefined) {
        this.pending = false;
    }
}

function create() {
    return new TaskQueue();
}
