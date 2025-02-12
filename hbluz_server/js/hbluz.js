var app = angular.module('myApp', ['chart.js', 'ngMaterial']);

app.controller('myCtrl', function($scope, $http, $filter, $interval) {
    //
    //Setup
    //


    //Placeholder for IP:Port
    $scope.ip = "127.0.0.1";
    $scope.port = "80";

    $scope._SendIcon = "nc";

    var LEDChannels = {
        UVB: 0,
        ROYAL_BLUE: 1,
        TRUE_GREEN: 2,
        HYPER_RED: 3,
        FAR_RED: 4,
        STREET_WHITE: 5
    };

    var emptyLightsetting = {
        "Description": "Unbenannt",
        "RelativeStart": 0,
        "Duration": 0,
        "TimeStart": "00:00",
        "TimeEnd": "00:00",
        "ChannelID": LEDChannels.ROYAL_BLUE,
        "PWMPointA": 0,
        "PWMPointB": 0
    }

    $scope.selectedLightsetting = emptyLightsetting;
    //If checked, relative dates (RelativeStart and Duration) will be updated when ProgramDateStart changes
    $scope._AutoUpdateDates = {
        Checked: true
    };



    $scope.LEDLookup = [{
            ChannelID: LEDChannels.UVB,
            Wavelength: 300,
            TrivialName: "UV-B"
        },
        {
            ChannelID: LEDChannels.ROYAL_BLUE,
            Wavelength: 440,
            TrivialName: "Royal Blue"
        },
        {
            ChannelID: LEDChannels.TRUE_GREEN,
            Wavelength: 530,
            TrivialName: "True Green"
        },
        {
            ChannelID: LEDChannels.HYPER_RED,
            Wavelength: 660,
            TrivialName: "Hyper Red"
        },
        {
            ChannelID: LEDChannels.FAR_RED,
            Wavelength: 730,
            TrivialName: "Far Red"
        },
        {
            ChannelID: LEDChannels.STREET_WHITE,
            Wavelength: 6500,
            TrivialName: "Street White"
        }
    ];


    ///
    //Events
    ///

    //
    $scope.selectedUserChanged = function() {
        $http.get("./php/get-programs.php?user_id=" + $scope.selectedUser.id).then(function(response) {
            $scope.programs = response.data;
        });
    };

    //
    $scope.selectedProgramChanged = function() {
        $scope.rawJSON = JSON.parse($scope.selectedProgram.raw_json);
        $scope._ProgramDateStart = new Date($scope.rawJSON.Program.ProgramDateStart);
        updateSPECChart();
    };

    $scope.UploadProgramToDB = uploadProgramToDB;

    function uploadProgramToDB(userID, programName, rawJSON, isAutosave) {
        var currentDateTime = Date.now();

        var dataObj = {
            user_id: userID,
            program_name: programName,
            is_autosave: isAutosave,
            date_posted: currentDateTime,
            raw_json: rawJSON
        };
        $http.post('./php/post-program.php', dataObj);

        $scope.selectedUserChanged();



    };

    $scope.DeleteProgramFromDB = function(program_id, program_name) {

        if (confirm("Are you sure you want to delete '" + program_name + "'?") == true) {
            var dataObj = {
                id: program_id
            };
            $http.post('./php/delete-program.php', dataObj);

            $scope.selectedUserChanged();


        }
    };

    $scope.programDateStartChanged = programDateStartChanged;
    $scope.relativeStartChanged = dateChanged;
    $scope.durationChanged = dateChanged;

    //Update JSON TimeStart when user inputs to correct format
    $scope.timeStartChanged = function() {
        $scope.selectedLightsetting.TimeStart = $filter('date')($scope._TimeStart, "HH:mm");
    };

    //Update JSON TimeEnd when user inputs to correct format
    $scope.timeEndChanged = function() {
        $scope.selectedLightsetting.TimeEnd = $filter('date')($scope._TimeEnd, "HH:mm");
    };

    //Parse dates correctly from JSON when user selects different lightsetting
    $scope.selectedLightsettingChanged = function() {
        $scope._TimeStart = new Date(new Date().toDateString() + ' ' + $scope.selectedLightsetting.TimeStart);
        $scope._TimeEnd = new Date(new Date().toDateString() + ' ' + $scope.selectedLightsetting.TimeEnd);
        updateRelativeDates();


        $scope._PWMPointA = $scope.selectedLightsetting.PWMPointA;
        $scope._PWMPointB = $scope.selectedLightsetting.PWMPointB;
        $scope._Intensity = Math.round(($scope._PWMPointB - $scope._PWMPointA) * 100 / 4095);
        updatePWMChart();
    };

    //Set IP and Port for connection when user selects different device
    $scope.selectedDeviceChanged = function() {
        $scope.ip = $scope.selectedDevice.IP;
        $scope.port = $scope.selectedDevice.Port;
    };

    //Update JSON ProgramDateStart when user inputs to correct format and also check if relative dates (RelativeStart and Duration) should be updated
    function programDateStartChanged() {
        $scope.rawJSON.Program.ProgramDateStart = $filter('date')($scope._ProgramDateStart, "yyyy-MM-dd");

        if ($scope._AutoUpdateDates.Checked) {
            updateRelativeDates();
        }
    }

    //Update JSON Dates (RelativeStart and Duration) to display difference in days
    function dateChanged() {
        $scope.selectedLightsetting.RelativeStart = dateDiffInDays($scope._ProgramDateStart, $scope._RelativeStart); //$filter('date')($scope._RelativeStart, "yyyy-MM-dd");
        $scope.selectedLightsetting.Duration = dateDiffInDays($scope._RelativeStart, $scope._Duration);
    }

    //Update relative dates (RelativeStart and Duration) depending on ProgramDateStart
    function updateRelativeDates() {
        var temp = new Date($scope._ProgramDateStart.toDateString());
        temp.setDate($scope._ProgramDateStart.getDate() + $scope.selectedLightsetting.RelativeStart);
        $scope._RelativeStart = temp;

        temp = new Date($scope._RelativeStart.toDateString());
        temp.setDate($scope._RelativeStart.getDate() + $scope.selectedLightsetting.Duration);
        $scope._Duration = temp;
    }

    //Update JSON intensity when user inputs intensity
    $scope.intensityChanged = function() {
        $scope._PWMPointB = Math.round((($scope._Intensity / 100) * 4095) + $scope._PWMPointA * 1);
        $scope.selectedLightsetting.PWMPointA = $scope._PWMPointA;
        $scope.selectedLightsetting.PWMPointB = $scope._PWMPointB;
        updatePWMChart();
        updateSPECChart();
    };

    $scope.PWMPointChangedAB = function() {
        $scope._Intensity = Math.round((($scope._PWMPointB - $scope._PWMPointA) / 4095) * 100);
        $scope.selectedLightsetting.PWMPointA = $scope._PWMPointA;
        $scope.selectedLightsetting.PWMPointB = $scope._PWMPointB;
        updatePWMChart();
        updateSPECChart();
    };
    $scope.TempChangedTopBottom = function() {
        updateTempChart();
    };

    $scope.wavelengthChanged = function() {
        updateSPECChart();
        $scope.selectedLightsetting.ChannelID = selectedLightsetting.ChannelID;
    };
    //Add new empty lightsetting to JSON
    $scope.addNewLightsetting = function() {
        $scope.rawJSON.Program.Lightsettings.Lightsetting.push(angular.copy(emptyLightsetting));
        updateSPECChart();
    };

    //Add new empty lightsetting to JSON
    $scope.duplicateLightsetting = function() {
        $scope.rawJSON.Program.Lightsettings.Lightsetting.push(angular.copy($scope.selectedLightsetting));
        updateSPECChart();
    };

    //Remove selected lightsetting from JSON
    $scope.removeLightsetting = function(selectedItem) {
        if (typeof selectedItem.Description == 'undefined') {
            return;
        }

        $scope.lastDeletedLightseting = selectedItem;
        var index = $scope.rawJSON.Program.Lightsettings.Lightsetting.indexOf(selectedItem);
        $scope.rawJSON.Program.Lightsettings.Lightsetting.splice(index, 1);
        updateSPECChart();
    };

    //Undo the removal of the last deleted lightsetting
    $scope.undoLastLightsetting = function() {
        if ($scope.lastDeletedLightseting == null) {
            return;
        }

        $scope.rawJSON.Program.Lightsettings.Lightsetting.push($scope.lastDeletedLightseting);
        $scope.lastDeletedLightseting = null;
    };

    //Update SyncTime and SyncDate in JSON to local time
    $scope.syncTime = UpdateSyncTime;

    function UpdateSyncTime() {
        $scope.rawJSON.Program.SyncDate = $filter('date')(new Date(), "yyyy-MM-dd");
        $scope.rawJSON.Program.SyncTime = $filter('date')(new Date(), "HH:mm:ss");
    };

    //Connect to client and GET JSON
    $scope.connect = function(ip, port) {
        $http.get("http://" + ip + ":" + port + "/getProgram").then(

            function successCallback(response) {
                $scope.rawJSON = response.data;
                $scope._ProgramDateStart = new Date($scope.rawJSON.Program.ProgramDateStart);
                TempTick(tickCounter);
                $interval(function() { TempTick(tickCounter) }, 10000);
                updateSPECChart();
            },
            function errorCallback(response) {
                // called asynchronously if an error occurs
                // or server returns response with an error status.
            }
        )

    };

    //Populate device list
    $scope.searchDevices = function() {
        $http.get("./dhcp/dnsmasq.leases").then(function(response) {
            var sampleLeases = response.data;
            var res = sampleLeases.split("\n");
            var deviceList = [];

            res.forEach(function(device) {
                var emptyDevice = {
                    "IP": "Unbenannt",
                    "Port": 80,
                    "DeviceName": "...",
                    "ProgramName": "...",
                    "Status": "..."
                }
                emptyDevice.IP = device.split(' ')[2];

                $http.get("http://" + emptyDevice.IP + ":" + emptyDevice.Port + "/getStatus").then(

                    function successCallback(response) {
                        emptyDevice.DeviceName = response.data.DeviceName;
                        emptyDevice.ProgramName = response.data.ProgramName;
                        emptyDevice.Status = response.data.Status;
                        emptyDevice.PasswordProtected = response.data.PasswordProtected;

                        // switch(response.data.Status)
                        // {   
                        //     case 0: emptyDevice.Status = "Ready"; break;
                        //     case 1: emptyDevice.Status = "Running"; break;
                        //     case 2: emptyDevice.Status = "Finished";break;
                        //     default: emptyDevice.Status = "Unknown";break;
                        // }

                        // if (response.data.PasswordProtected == 0) {
                        //     emptyDevice.PasswordProtected = "";
                        // } else {
                        //     emptyDevice.PasswordProtected = "Password Protected";
                        // }
                        deviceList.push(emptyDevice);
                    },
                    function errorCallback(response) {
                        // called asynchronously if an error occurs
                        // or server returns response with an error status.
                    }
                )


            });
            $scope.deviceList = deviceList;


        })
    };

    //POST JSON to client
    $scope.postJSON = function(isAutosave) {
        UpdateSyncTime();
        $scope._SendIcon = "sn";

        $http.get("./php/get-hash.php?pwd=" + $scope._ProgramPassword).then(function(response) {
            $scope.returnedPwdHash = response.data;
            //$scope.rawJSON.Program.Password = $scope.returnedPwdHash;
            $scope._SendIcon = "dn";
            var req = {
                method: 'POST',
                url: "http://" + $scope.ip + ":" + $scope.port + "/postProgram",
                headers: {
                    'Content-Type': "text/plain" //text/plain
                },
                data: $scope.rawJSON
            };

            $http(req);
            $scope._SendIcon = "db";
        });


        uploadProgramToDB($scope.selectedUser.id, $scope.rawJSON.Program.ProgramName, $scope.rawJSON, isAutosave);

    };

    ///
    //Helper Functions
    ///

    //Returns difference in days between two Dates
    function dateDiffInDays(a, b) {
        var _MS_PER_DAY = 1000 * 60 * 60 * 24;
        // Discard the time and time-zone information.
        var utc1 = Date.UTC(a.getFullYear(), a.getMonth(), a.getDate());
        var utc2 = Date.UTC(b.getFullYear(), b.getMonth(), b.getDate());
        return Math.floor((utc2 - utc1) / _MS_PER_DAY);
    }

    //    
    //Main
    //

    $scope.searchDevices();
    $http.get("./php/get-users.php").then(function(response) {
        $scope.users = response.data;
    });


    //
    //Chart
    //
    function updatePWMChart() {
        $scope.PWMdata = [
            [{
                x: 0,
                y: 0
            }, {
                x: $scope.selectedLightsetting.PWMPointA,
                y: 0
            }, {
                x: $scope.selectedLightsetting.PWMPointA,
                y: 5
            }, {
                x: $scope.selectedLightsetting.PWMPointB,
                y: 5
            }, {
                x: $scope.selectedLightsetting.PWMPointB,
                y: 0
            }, {
                x: 4095,
                y: 0
            }]
        ];
    }
    $scope.PWMseries = ['PWM'];
    $scope.PWMdata = [{

        data: [{
            x: 0,
            y: 0
        }, {
            x: 4095,
            y: 0
        }]
    }];
    $scope.PWMoptions = {
        responsive: true,
        title: {
            display: true,
            text: "Pulse-width modulation",
            fontSize: 20,
            fontColor: "#CCC"
        },
        elements: {
            line: {
                tension: 0
            }
        },
        scales: {
            xAxes: [{
                id: '1',
                type: 'linear',
                position: 'bottom',
                scaleLabel: {
                    display: true,
                    labelString: 'Time',
                    fontColor: "#CCC",
                    fontSize: 15
                },
                gridLines: {
                    display: true,

                },
                ticks: {
                    fontColor: "#CCC",
                }
            }],
            yAxes: [{
                scaleLabel: {
                    display: true,
                    labelString: 'Electric potential [V]',
                    fontColor: "#CCC",
                    fontSize: 15
                },
                gridLines: {
                    display: true,

                },
                ticks: {
                    fontColor: "#CCC",
                }
            }]
        }

    };

    $scope.PWMcolors = [{
        backgroundColor: 'rgba(57, 255, 20,0.2)', //Color under data line
        borderColor: '#39FF14', // Color of data
        pointBorderColor: '#39FF14', //Color of data pionts
    }];

    function updateTempChart(ticktock) {
        $scope.TEMPdata[0].push({
            x: ticktock,
            y: $scope.TemperatureTop
        });

        $scope.TEMPdata[1].push({
            x: ticktock,
            y: $scope.TemperatureBottom
        });

    }

    $scope.TEMPseries = ['Temp Top', 'Temp Bottom'];
    $scope.TEMPdata = [
        [{}],
        [{}]
    ];
    $scope.TEMPcolors = ['#39FF14', '#0404B4'];

    $scope.TEMPoptions = {
        responsive: true,
        title: {
            display: true,
            text: "Temperature Trend",
            fontSize: 20,
            fontColor: "#CCC"
        },


        scales: {
            xAxes: [{
                id: '1',
                type: 'linear',
                position: 'bottom',
                scaleLabel: {
                    display: true,
                    labelString: 'Time [s]',
                    fontColor: "#CCC",
                    fontSize: 15
                },
                gridLines: {
                    display: true,
                    color: 'rgba(255,255,255,1)'
                },
                ticks: {
                    fontColor: "#CCC",
                }
            }],
            yAxes: [{
                id: 'y-axis-1',
                type: 'linear',
                position: 'left',
                scaleLabel: {
                    display: true,
                    labelString: 'Temperature [C]',
                    fontColor: "#CCC",
                    fontSize: 15
                },
                ticks: { suggestedMin: 0, suggestedMax: 40 }
            }]
        },
        elements: {
            line: {
                fill: false
            }

        }
    };

    function updateSPECChart() {
        $scope.SPECdata = [];
        $scope.SPECcolors = [];

        angular.forEach($scope.rawJSON.Program.Lightsettings.Lightsetting, function(value, key) {

            switch (value['ChannelID']) {
                case LEDChannels.UVB:
                    $scope.SPECdata.push([{
                            x: 271,
                            y: 0
                        },
                        {
                            x: 281,
                            y: (value['PWMPointB'] - value['PWMPointA']) * 5 / 4095
                        },
                        {
                            x: 297,
                            y: (value['PWMPointB'] - value['PWMPointA']) * 100 / 4095
                        },
                        {
                            x: 321,
                            y: (value['PWMPointB'] - value['PWMPointA']) * 5 / 4095
                        },
                        {
                            x: 332,
                            y: 0
                        }
                    ]);
                    $scope.SPECcolors.push('#6A0888');
                    break;

                case LEDChannels.ROYAL_BLUE:
                    $scope.SPECdata.push([{
                            x: 400,
                            y: 0
                        },
                        {
                            x: 418,
                            y: (value['PWMPointB'] - value['PWMPointA']) * 5 / 4095
                        },

                        {
                            x: 444,
                            y: (value['PWMPointB'] - value['PWMPointA']) * 100 / 4095
                        },

                        {
                            x: 473,
                            y: (value['PWMPointB'] - value['PWMPointA']) * 5 / 4095
                        },
                        {
                            x: 520,
                            y: 0
                        }
                    ]);
                    $scope.SPECcolors.push('#0404B4');
                    break;

                case LEDChannels.TRUE_GREEN:
                    $scope.SPECdata.push([{
                            x: 470,
                            y: 0
                        },
                        {
                            x: 488,
                            y: (value['PWMPointB'] - value['PWMPointA']) * 5 / 4095
                        },
                        {
                            x: 526,
                            y: (value['PWMPointB'] - value['PWMPointA']) * 100 / 4095
                        },
                        {
                            x: 579,
                            y: (value['PWMPointB'] - value['PWMPointA']) * 5 / 4095
                        },
                        {
                            x: 615,
                            y: 0
                        }
                    ]);
                    $scope.SPECcolors.push('#31B404');
                    break;

                case LEDChannels.HYPER_RED:
                    $scope.SPECdata.push([{
                            x: 600,
                            y: 0
                        },

                        {
                            x: 625,
                            y: (value['PWMPointB'] - value['PWMPointA']) * 5 / 4095
                        },
                        {
                            x: 659,
                            y: (value['PWMPointB'] - value['PWMPointA']) * 100 / 4095
                        },

                        {
                            x: 678,
                            y: (value['PWMPointB'] - value['PWMPointA']) * 5 / 4095
                        },
                        {
                            x: 700,
                            y: 0
                        }
                    ]);
                    $scope.SPECcolors.push('#FF0000');
                    break;

                case LEDChannels.FAR_RED:
                    $scope.SPECdata.push([{
                            x: 650,
                            y: 0
                        },
                        {
                            x: 681,
                            y: (value['PWMPointB'] - value['PWMPointA']) * 5 / 4095
                        },
                        {
                            x: 731,
                            y: (value['PWMPointB'] - value['PWMPointA']) * 100 / 4095
                        },
                        {
                            x: 758,
                            y: (value['PWMPointB'] - value['PWMPointA']) * 5 / 4095
                        },
                        {
                            x: 780,
                            y: 0
                        }
                    ]);
                    $scope.SPECcolors.push('#B40431');
                    break;

                case LEDChannels.STREET_WHITE:
                    $scope.SPECdata.push([{
                            x: 400,
                            y: 0
                        },
                        {
                            x: 415,
                            y: (value['PWMPointB'] - value['PWMPointA']) * 5 / 4095
                        },
                        {
                            x: 441,
                            y: (value['PWMPointB'] - value['PWMPointA']) * 100 / 4095
                        },
                        {
                            x: 476,
                            y: 6.5 * ((value['PWMPointB'] - value['PWMPointA']) * 100 / 4095) / 100
                        },
                        {
                            x: 557,
                            y: 35 * ((value['PWMPointB'] - value['PWMPointA']) * 100 / 4095) / 100
                        },
                        {
                            x: 677,
                            y: (value['PWMPointB'] - value['PWMPointA']) * 5 / 4095
                        },
                        {
                            x: 755,
                            y: 0
                        }
                    ]);
                    $scope.SPECcolors.push('#333333');
                    break;
                default:
                    break;
            }
        });

    }
    $scope.SPECseries = ['SPEC'];
    $scope.SPECdata = [
        [{
            x: 0,
            y: 0
        }]
    ];
    $scope.SPECcolors = ['#000'];
    $scope.SPECoptions = {
        responsive: true,
        title: {
            display: true,
            text: "Spectral power distribution",
            fontSize: 20,
            fontColor: "#CCC"
        },


        scales: {
            xAxes: [{
                id: '1',
                type: 'linear',
                position: 'bottom',
                scaleLabel: {
                    display: true,
                    labelString: 'Wavelength [nm]',
                    fontColor: "#CCC",
                    fontSize: 15
                },
                gridLines: {
                    display: true,
                    color: 'rgba(255,255,255,1)'
                },
                ticks: {
                    fontColor: "#CCC",
                }
            }],
            yAxes: [{
                scaleLabel: {
                    display: true,
                    labelString: 'Rel. Intensity [%]',
                    fontColor: "#CCC",
                    fontSize: 15
                },
                gridLines: {
                    display: true,
                    color: 'rgba(255,255,255,1)'
                },
                ticks: {
                    fontColor: "#CCC",
                }
            }]
        }
    };



    // $scope.SPECcolors = [{ // grey
    //   backgroundColor: 'rgba(57, 255, 20,0.2)',
    //pointBackgroundColor: 'rgba(148,159,177,1)',
    //pointHoverBackgroundColor: 'rgba(148,159,177,1)',
    //borderColor: ['#000','#000','#000','#000','#000','#000'],
    // pointBorderColor: '#fff',
    //pointHoverBorderColor: 'rgba(148,159,177,0.8)'
    //}];

    var TimeTick = function() {
        $scope.clock = Date.now();
    }
    TimeTick();
    $interval(TimeTick, 1000);

    var tickCounter = 0;
    var TempTick = function(ticktock) {
        $http.get("http://" + $scope.ip + ":" + $scope.port + "/getTemperature").then(

            function successCallback(response) {

                $scope.Temperature1 = response.data.Temperature1;
                $scope.Temperature2 = response.data.Temperature2;
                $scope.Temperature3 = response.data.Temperature3;
                $scope.Temperature4 = response.data.Temperature4;

                $scope.TemperatureTop = (($scope.Temperature1 + $scope.Temperature2) / 2).toFixed(2);
                $scope.TemperatureBottom = (($scope.Temperature3 + $scope.Temperature4) / 2).toFixed(2);
                updateTempChart(ticktock);

            },
            function errorCallback(response) {
                // called asynchronously if an error occurs
                // or server returns response with an error status.
            }
        )

        tickCounter += 10;
    }



});

app.controller('TimeCtrl', function($scope, $interval) {

});


// router
var app2 = angular.module("router", ["ngRoute"]);
app2.config(function($routeProvider) {
    $routeProvider
        .when("/home", {
            templateUrl: "home.html"
        })
        .when("/user", {
            templateUrl: "user.html"
        })
        .when("/settings", {
            templateUrl: "settings.html"
        });
});

// necessary when having multilple apps (modules?)
angular.bootstrap(document.getElementById("routerApp"), ['router']);