var eid = require('./build/Release/addon');

var res = eid.generateEID("e2e12c2281cdf3d350a34de4d5f56613", 0, 2352222);
console.log(res.eid);
module.exports = eid;
