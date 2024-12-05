int cabinY;            // Position of the elevator cabin
int targetY;           // Target position for the cabin
boolean doorsOpen;     // Whether the doors are open
boolean moving;        // Whether the cabin is moving
int floorHeight = 192; // Height of each floor
int numFloors = 3;     // Total number of floors
int currentFloor;      // Current floor of the elevator

void setup() {
  size(800, 800);       // Window size
  cabinY = height - 250; // Start at the bottom floor
  targetY = cabinY;
  doorsOpen = false;
  moving = false;
  currentFloor = 1;     // Start on the first floor
}

void draw() {
  background(220, 240, 255); // Light blue background

  // Elevator shaft
  fill(60, 60, 60);
  rect(200, 150, 200, height - 195);

  // Floors
  fill(80);
  for (int i = 0; i <= numFloors; i++) {
    rect(200, 150 + i * floorHeight, 200, 30);
  }

  // Elevator cabin
  fill(180, 180, 200); // Light gray
  rect(200, cabinY, 200, floorHeight);
  fill(100, 100, 150); // Slightly darker for interior
  rect(210, cabinY + 10, 180, floorHeight - 20);

  // Doors
  if (doorsOpen) {
    fill(100, 200, 100); // Green for open doors
    rect(200, cabinY, 70, floorHeight);    // Left door
    rect(330, cabinY, 70, floorHeight);    // Right door
  } else {
    fill(50, 50, 100); // Blue for closed doors
    rect(200, cabinY, 100, floorHeight);    // Closed left door
    rect(300, cabinY, 100, floorHeight);    // Closed right door
  }

  // Simulate cabin movement
  if (moving) {
    if (abs(cabinY - targetY) > 2) { // Smooth transition to target
      cabinY += (targetY - cabinY) / abs(targetY - cabinY) * 2; // Move incrementally
    } else {
      cabinY = targetY; // Snap to target
      moving = false; // Stop movement
      currentFloor = numFloors - (cabinY - 50) / floorHeight; // Update floor
    }
  }

  // Draw buttons
  drawButtons();

  // Draw 7-segment displays
  drawSevenSegmentDisplay(500, 200, currentFloor); // Top display
  drawSevenSegmentDisplay(500, 400, currentFloor); // middle display
  drawSevenSegmentDisplay(500, height - 200, currentFloor); // Bottom display
}

void drawButtons() {
  textSize(18);
  textAlign(CENTER, CENTER);
  
  // UP Button
  fill(0, 200, 0);
  rect(50, height - 250, 100, 40);
  fill(255);
  text("UP", 100, height - 230);
  
  // DOWN Button
  fill(200, 0, 0);
  rect(50, height - 200, 100, 40);
  fill(255);
  text("DOWN", 100, height - 180);
  
  // TOGGLE DOORS Button
  fill(0, 0, 200);
  rect(50, height - 150, 100, 40);
  fill(255);
  text("DOORS", 100, height - 130);
}

void drawSevenSegmentDisplay(int x, int y, int number) {
  // Coordinates for segments
  int segWidth = 10;
  int segHeight = 20;
  int spacing = 5;
  
  // Colors
  fill(30);
  rect(x, y, segWidth * 3 + spacing * 2, segHeight * 2 + spacing * 4, 6); // Outer display

  fill(255, 0, 0); // Red for lit segments
  
  // Segment positions (lit based on number)
  boolean[] segments = new boolean[7];
  switch (number) {
    case 1: segments = new boolean[] {false, true, true, false, false, false, false}; break;
    case 2: segments = new boolean[] {true, true, false, true, true, false, true}; break;
    case 3: segments = new boolean[] {true, true, true, true, false, false, true}; break;
  }

  // Draw segments based on state
  if (segments[0]) rect(x + segWidth + spacing, y, segWidth, spacing); // Top
  if (segments[1]) rect(x + 2 * segWidth + 2 * spacing, y + spacing, spacing, segHeight); // Top-right
  if (segments[2]) rect(x + 2 * segWidth + 2 * spacing, y + 2 * spacing + segHeight, spacing, segHeight); // Bottom-right
  if (segments[3]) rect(x + segWidth + spacing, y + 2 * (spacing + segHeight), segWidth, spacing); // Bottom
  if (segments[4]) rect(x, y + 2 * spacing + segHeight, spacing, segHeight); // Bottom-left
  if (segments[5]) rect(x, y + spacing, spacing, segHeight); // Top-left
  if (segments[6]) rect(x + segWidth + spacing, y + spacing + segHeight, segWidth, spacing); // Middle
}

void mousePressed() {
  if (mouseX > 50 && mouseX < 150) {
    if (mouseY > height - 250 && mouseY < height - 210) { // UP Button
      if (cabinY - floorHeight >= 50) {
        targetY = cabinY - floorHeight; // Move up by one floor
        moving = true;
        doorsOpen = false; // Close doors while moving
      }
    } else if (mouseY > height - 200 && mouseY < height - 160) { // DOWN Button
      if (cabinY + floorHeight <= height - 200) {
        targetY = cabinY + floorHeight; // Move down by one floor
        moving = true;
        doorsOpen = false; // Close doors while moving
      }
    } else if (mouseY > height - 150 && mouseY < height - 110) { // TOGGLE DOORS Button
      if (!moving) { // Allow door toggle only when stopped
        doorsOpen = !doorsOpen; // Toggle doors
      }
    }
  }
}
