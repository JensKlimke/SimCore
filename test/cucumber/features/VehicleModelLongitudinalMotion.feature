# language: en
Feature: Longitudinal Motion Model

  Background:
    Given the vehicle model with time step size 0.01 s
    And the vehicle at position (0 m, 0 m)
    And the vehicle's shifter is set to D

  @final-state
  Scenario: Constant Motion (no acceleration)
    Given the vehicle has a speed of 10 m/s
    And a yaw angle of 22.5 degrees
    And the vehicle at position (100 m, 200 m)
    When the simulation has been ran for 10 seconds
    Then the position of the vehicle shall be x=192.387 m and y=238.268 m

  @continuous-state
  Scenario: Accelerated Motion
    Given the vehicle has a speed of 10 m/s
    And the vehicle's external relative force is 0.1 N/kg
    When the simulation is executed for 10 seconds
    Then the vehicle's acceleration profile shall be const(0.1)
    And the vehicle's velocity profile shall be poly(10, 0.1)
    And the vehicle's distance profile shall be poly(0, 10, 0.05)

