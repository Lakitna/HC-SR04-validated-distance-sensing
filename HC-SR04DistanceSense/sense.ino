void senseUpdate(NewPing sense) {
  unsigned int cm = sense.ping() / US_ROUNDTRIP_CM; // Get distance in centimeters (cm)

  // Scale pos on exponential scale
  int pos = fscale(
      cm,
      SEN_MIN_DISTANCE,
      SEN_MAX_DISTANCE,
      SER_MIN_RANGE,
      SER_MAX_RANGE,
      SCALE_CURVE
    );
  
  // If 0cm, pos = infinity
  if (cm == 0) {
    digitalWrite(13, HIGH); // Visualise drops

    pos = SER_MAX_RANGE;    // Set pos to infinity 


    // Prepare servo update check 1, inf version
    count = 0; // Empty counter
    for (int i=0; i<INF_LOOPS; i++) { // For int infLoops do:
      // Count++ if previous val passes test
      if (position_prev[i] == SER_MAX_RANGE) count++; // If val == max
    }
    if (count == INF_LOOPS) update = true;  // If test was passed every loop do update
    else                    update = false; // Else don't update
    
  }
  else {
    digitalWrite(13, LOW); // Visualise drops

    // Prepare servo update check 1
    count = 0; // Empty counter
    for (int i=0; i<LOOPS; i++) { // For int loops do:
      // Calculate difference between current and previous vals
      int dif = (pos - position_prev[i]);
      
      // Count++ if previous val passes test
      if (pos == position_prev[i]) count++; // If same as prev
      else if (dif < POS_MAX_CHANGE && dif > POS_VARIANCE)      count++; // If close to prev
      else if (dif < POS_VARIANCE   && dif > -1*POS_MAX_CHANGE) count++; // If close to prev
    }
    if (count == LOOPS) update = true;  // If test was passed every loop do update
    else                update = false; // Else don't update
    
  }



  
  Serial.print("Checks passed: ");
  
  // Servo update test 1
  if (update) {
    Serial.print(1); // Test 1 passed
    
    // Servo update test 2, same as previous servo value?
    if (ser_prev != pos) {
      Serial.print(2); // Test 2 passed

      // Servo update test 3, how big is the difference with previous servo value?
      int dif = ser_prev - pos;
      if (dif > SER_VARIANCE || dif < -1*SER_VARIANCE) {
        Serial.print(3); // Test 3 passed
        
        ser.write(pos); // Update servo
    
        ser_prev = pos; // Store value as previous value
      }
      else {
        // If test failed
        Serial.print("\tDrop: Servo variance");
      }
      
    }
    else {
      // If test failed
      Serial.print("\tDrop: Same servo val");
    }
    
  }
  else {
    // If test failed
    Serial.print("\tDrop: Update val ");
    Serial.print(count);
  }
  
  Serial.println();


  // Update previous position array
  for (int i=(INF_LOOPS-1); i>=0; i--) {    
    if (i==0) position_prev[i] = pos;
    else      position_prev[i] = position_prev[i-1];
  }

}

