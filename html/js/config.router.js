'use strict';

/**
 * Config for the router
 */
angular.module('app')
  .run(
    [ '$cookies', '$rootScope', '$state', '$stateParams',
      function ($cookies, $rootScope, $state, $stateParams) {
          $rootScope.$state = $state;
          $rootScope.$stateParams = $stateParams;

          // add by xxu1 for Auth Login
          $rootScope.$on('$stateChangeStart', function(event, toState) {
            if (angular.isString($cookies.user) && $cookies.user.length >= 3) {
              //console.log('user: ' + $cookies.user);
            } else {    
              if (toState.name != 'access.signin') {
                //console.log('unAuth, jump Login');
                $state.go('access.signin', {reload: true});
                event.preventDefault();
              }
            }
          });
      }
    ]
  )
  .config(
    [          '$stateProvider', '$urlRouterProvider',
      function ($stateProvider,   $urlRouterProvider) {
          
          $urlRouterProvider
              .otherwise('/app/dashboard');
          $stateProvider
              .state('app', {
                  abstract: true,
                  url: '/app',
                  templateUrl: 'tpl/app.html'
              })
              .state('app.dashboard', {
                  url: '/dashboard',
                  templateUrl: 'tpl/app_dashboard.html',
              })
              .state('app.SystemView', {
                  url: '/SystemView',
                  templateUrl: 'tpl/SystemView.html',
                  params: {'id': 0},
              })
              .state('app.ShelfView', {
                  url: '/ShelfView',
                  templateUrl: 'tpl/ShelfView.html',
                  params: {'id': 0},
              })
              .state('app.CardView', {
                  url: '/CardView',
                  templateUrl: 'tpl/CardView.html',
                  params: {'id': 0},
              })
              .state('app.PsuTable', {
                  url: '/PsuTable',
                  templateUrl: 'tpl/PsuTable.html'
              })
              .state('app.PsuView', {
                  url: '/PsuView',
                  templateUrl: 'tpl/PsuView.html',
                  params: {'id': null},
              })
              .state('app.FanTable', {
                  url: '/FanTable',
                  templateUrl: 'tpl/FanTable.html'
              })
              .state('app.FanView', {
                  url: '/FanView',
                  templateUrl: 'tpl/FanView.html',
                  params: {'id': null},
              })
              .state('app.OpticsTable', {
                  url: '/OpticsTable',
                  templateUrl: 'tpl/OpticsTable.html'
              })
              .state('app.OpticsView', {
                  url: '/OpticsView',
                  templateUrl: 'tpl/OpticsView.html',
                  params: {'id': null},
              })
              .state('app.NetworkTable', {
                  url: '/NetworkTable',
                  templateUrl: 'tpl/NetworkTable.html'
              })
              .state('app.NetworkView', {
                  url: '/NetworkView',
                  templateUrl: 'tpl/NetworkView.html',
                  params: {'id': null},
              })
              .state('app.PortTable', {
                  url: '/PortTable',
                  templateUrl: 'tpl/PortTable.html'
              })
              .state('app.PortView', {
                  url: '/PortView',
                  templateUrl: 'tpl/PortView.html',
                  params: {'id': null},
              })
              .state('app.OtuTable', {
                  url: '/OtuTable',
                  templateUrl: 'tpl/OtuTable.html'
              })
              .state('app.OtuView', {
                  url: '/OtuView',
                  templateUrl: 'tpl/OtuView.html',
                  params: {'id': null},
              })
              .state('app.OchTable', {
                  url: '/OchTable',
                  templateUrl: 'tpl/OchTable.html'
              })
              .state('app.OchView', {
                  url: '/OchView',
                  templateUrl: 'tpl/OchView.html',
                  params: {'id': null},
              })
              .state('app.EthTable', {
                  url: '/EthTable',
                  templateUrl: 'tpl/EthTable.html'
              })
              .state('app.EthView', {
                  url: '/EthView',
                  templateUrl: 'tpl/EthView.html',
                  params: {'id': null},
              })
              .state('app.InventoryTable', {
                  url: '/InventoryTable',
                  templateUrl: 'tpl/InventoryTable.html'
              })
              .state('app.InventoryView', {
                  url: '/InventoryView',
                  templateUrl: 'tpl/InventoryView.html',
                  params: {'id': null},
              })
              .state('app.AlarmTable', {
                  url: '/AlarmTable',
                  templateUrl: 'tpl/AlarmTable.html'
              })
              .state('app.CounterTable', {
                  url: '/CounterTable',
                  templateUrl: 'tpl/CounterTable.html'
              })
              .state('app.CounterView', {
                  url: '/CounterView',
                  templateUrl: 'tpl/CounterView.html',
                  params: {'id': null},
              })
              .state('app.FileMgmt', {
                  url: '/FileMgmt',
                  templateUrl: 'tpl/FileMgmtView.html',
                  params: {'id': 0},
              })
              .state('app.UserAdminTable', {
                  url: '/UserAdminTable',
                  templateUrl: 'tpl/UserAdminTable.html'
              })
              .state('app.UserAdminView', {
                  url: '/UserAdminView',
                  templateUrl: 'tpl/UserAdminView.html',
                  params: {'id': null},
              })
              .state('app.Snmp', {
                  url: '/Snmp',
                  templateUrl: 'tpl/SnmpView.html',
              })
              .state('app.ui', {
                  url: '/ui',
                  template: '<div ui-view class="fade-in-up"></div>'
              })
              .state('app.ui.buttons', {
                  url: '/buttons',
                  templateUrl: 'tpl/ui_buttons.html'
              })
              .state('app.ui.icons', {
                  url: '/icons',
                  templateUrl: 'tpl/ui_icons.html'
              })
              .state('app.ui.grid', {
                  url: '/grid',
                  templateUrl: 'tpl/ui_grid.html'
              })         
              .state('app.ui.bootstrap', {
                  url: '/bootstrap',
                  templateUrl: 'tpl/ui_bootstrap.html'
              })
              .state('app.ui.toaster', {
                  url: '/toaster',
                  templateUrl: 'tpl/ui_toaster.html',
                  resolve: {
                      deps: ['$ocLazyLoad',
                        function( $ocLazyLoad){
                          return $ocLazyLoad.load('toaster').then(
                              function(){
                                 return $ocLazyLoad.load('js/controllers/toaster.js');
                              }
                          );
                      }]
                  }
              })
              // pages
              .state('app.docs', {
                  url: '/docs',
                  templateUrl: 'tpl/docs.html'
              })
              // others
              .state('access', {
                  url: '/access',
                  template: '<div ui-view class="fade-in-right-big smooth"></div>'
              })
              .state('access.signin', {
                  url: '/signin',
                  templateUrl: 'tpl/page_signin.html',
                  resolve: {
                      deps: ['uiLoad',
                        function( uiLoad ){
                          return uiLoad.load( ['js/controllers/signin.js'] );
                      }]
                  }
              })
      }
    ]
  );
