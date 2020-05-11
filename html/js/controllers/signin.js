'use strict';

/* Controllers */
  // signin controller
app.controller('SigninFormController', ['$cookies', '$scope', '$http', '$state', function($cookies, $scope, $http, $state) {
    $scope.user = {};
    $scope.authError = null;
    $cookies.user = '';
    $scope.login = function() {
      $scope.authError = null;
      // Try to login
      //$http.post('/action/Login', {username: $scope.user.username, password: $scope.user.password})
      $http.post('/action/Login?username=' + $scope.user.username + '&password=' + $scope.user.password)
      .then(function(response) {
        //console.info('response', response);
        if ( !response.data.success ) {
          $scope.authError = 'Username or Password Incorrect';
        }else{
          $cookies.user = $scope.user.username;
          $state.go('app.dashboard');
        }
      }, function(x) {
        $scope.authError = 'Server Error';
      });
    };
  }])
;