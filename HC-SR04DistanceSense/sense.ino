int senseUpdate(NewPing sense) {
  unsigned int cm = sense.ping() / US_ROUNDTRIP_CM; // Get distance in centimeters (cm)

  int pos = cm;
  
  // If 0cm, pos = infinity
  if (cm == 0) {
    pos = SCALED_MAX_RANGE; // Set pos to max servo val

    // Prepare servo update check 1, inf version
    count = 0; // Reset counter
    for (byte i=0; i<INF_LOOPS; i++) { // For int infLoops do:
      // Count++ if previous val passes test
      if (position_prev[i] == pos) count++; // If pos == max
    }
    if (count == INF_LOOPS) update = true;  // If test was passed every loop do update
    else                    update = false; // Else don't update
  }
  else {
    // Scale sensor output on exponential scale
    pos = fscale(
        cm,
        SEN_MIN_DISTANCE,
        SEN_MAX_DISTANCE,
        SCALED_MIN_RANGE,
        SCALED_MAX_RANGE,
        SCALE_CURVE
      );

    // Prepare servo update check 1
    count = 0; // Reset counter
    for (byte i=0; i<LOOPS; i++) { // For int loops do:
      // Calculate difference between current and previous vals
      int dif = (pos - position_prev[i]);
      
      // Count++ if the difference between vals pass the test
      if (dif == 0) count++; // If same as prev
      else if (dif < SEN_MAX_CHANGE && dif > POS_VARIANCE)      count++; // If close to prev
      else if (dif < POS_VARIANCE   && dif > -1*SEN_MAX_CHANGE) count++; // If close to prev
    }
    if (count == LOOPS) update = true;  // If test was passed every loop do update
    else                update = false; // Else don't update
  }



  
  Serial.print("Checks passed: ");
  
  // Servo update test 1 (see above)
  // Check if the latest few values are equal or close to eachother
  if (update) {
    Serial.print("1"); // Test 1 passed
    
    // Servo update test 2, same as previous servo value?
    // Check if servo update is redundant because the value didn't change
    if (scaled_prev != pos) {
      Serial.print("-2"); // Test 2 passed

      // Servo update test 3, how big is the difference with previous servo value?
      int dif = scaled_prev - pos;
      if (dif > SCALED_VARIANCE || dif < -1*SCALED_VARIANCE) {
        Serial.println("-3"); // Test 3 passed
        
//        ser.write(pos); // Update servo
        scaled_prev = pos; // Store value as previous value
        return pos;     // Return value
      }
      else {
        // If test failed show debug
        Serial.print("\tDrop: Servo variance");
      }
      
    }
    else {
      // If test failed show debug
      Serial.print("\tDrop: Same servo val");
    }
    
  }
  else {
    // If test failed show debug
    Serial.print("\tDrop: Sensor test val ");
    Serial.print(count);
    Serial.print("/");
    if (cm == 0) Serial.print(INF_LOOPS);
    else         Serial.print(LOOPS);
  }
  
  Serial.print("\n");


  // Update previous position array
  for (int i=(INF_LOOPS-1); i>=0; i--) {    
    if (i==0) position_prev[i] = pos;
    else      position_prev[i] = position_prev[i-1];
  }
}

