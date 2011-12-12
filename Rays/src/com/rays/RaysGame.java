package com.rays;

import com.badlogic.gdx.ApplicationListener;
import com.badlogic.gdx.Gdx;
import com.badlogic.gdx.Input.Peripheral;
import com.badlogic.gdx.files.FileHandle;
import com.badlogic.gdx.graphics.GL10;
import com.badlogic.gdx.graphics.Pixmap;
import com.badlogic.gdx.graphics.Texture;
import com.badlogic.gdx.graphics.g2d.SpriteBatch;
import com.badlogic.gdx.graphics.g2d.TextureRegion;
import com.badlogic.gdx.input.RemoteInput;


//! Majority of original raycasting code from http://www.permadi.com/tutorial/raycast/index.html
public class RaysGame implements ApplicationListener {
	static final String TAG = "Rays";
	
	
	// size of tiles, etc
	static final int TILE_SIZE = 64;
	static final int WALL_HEIGHT = 64;
	static final int PROJECTION_PLANE_WIDTH = 320;
	static final int PROJECTION_PLANE_HEIGHT = 240;
	
	// position on projection plane that represents the angles
	static final int ANGLE60 = PROJECTION_PLANE_WIDTH;
	static final int ANGLE30 = (ANGLE60/2);
	static final int ANGLE15 = (ANGLE30/2);
	static final int ANGLE90 = (ANGLE30*3);
	static final int ANGLE180 = (ANGLE90*2);
	static final int ANGLE270 = (ANGLE90*3);
	static final int ANGLE360 = (ANGLE60*6);
	static final int ANGLE0 = 0;
	static final int ANGLE5 = (ANGLE30/6);
	static final int ANGLE10 = (ANGLE5*2);
	
 	// trigonometric tables
	float sinTable[];
	float invSinTable[];
	float cosTable[];
	float invCosTable[];
	float tanTable[];
	float invTanTable[];
	float fishTable[];
	float xStepTable[];
	float yStepTable[];	
	
	public static int playerX = 100;
	public static int playerY = 160;
	public static int playerAngle = ANGLE5;
	int playerDistanceToProjectionPlane = 277;
	int playerHeight = 32;
	int playerSpeed = 16;
	int projectionPlaneYCenter = PROJECTION_PLANE_HEIGHT / 2;
	int playerMapX, playerMapY;
	
	// 2 dimensional map
	byte fMap[];
	static final byte W=1;                                // wall
	static final byte O=0;                                // opening
	static final int MAP_WIDTH=12;
	static final int MAP_HEIGHT=12;
	
	Pixmap pixmap;
	Texture texture;
	SpriteBatch batch;
	TextureRegion region;
	Texture joystickBG;
	Texture joystick;
	
	Input input = new Input();
	
	@Override
	public void create() {
		//Gdx.input = new RemoteInput();
		Gdx.input.setInputProcessor(input);
		
		joystickBG = new Texture(Gdx.files.internal("data/joystick_bg.png"));
		joystick = new Texture(Gdx.files.internal("data/joystick.png"));
		
		
		// Create an empty dynamic pixmap
		pixmap = new Pixmap(PROJECTION_PLANE_WIDTH, PROJECTION_PLANE_HEIGHT, Pixmap.Format.RGBA8888);
		
		// Create a texture to contain the pixmap
		texture = new Texture(1024, 1024, Pixmap.Format.RGBA8888);
		texture.setFilter(Texture.TextureFilter.Nearest, Texture.TextureFilter.Linear);
		texture.setWrap(Texture.TextureWrap.ClampToEdge, Texture.TextureWrap.ClampToEdge);

		createTables();	

		// Blit the composited overlay to a texture
		texture.draw(pixmap, 0, 0);
		region = new TextureRegion(texture, 0, 0, PROJECTION_PLANE_WIDTH, PROJECTION_PLANE_HEIGHT);
		batch = new SpriteBatch();
	}

	@Override
	public void dispose() {
	}

	@Override
	public void pause() {
	}

	@Override
	public void render() {		
		drawBackground();
		castRays();
		texture.draw(pixmap, 0, 0);
		
		Gdx.gl.glClearColor(0.6f, 0.6f, 0.6f, 1);
		Gdx.gl.glClear(GL10.GL_COLOR_BUFFER_BIT);
		batch.begin();
		
		// Draw raycasted scene
		batch.draw(region, 0, 0, Gdx.graphics.getWidth(), Gdx.graphics.getHeight() );
		
		// Draw joystick (if necessary)
		batch.draw(joystickBG, 10, 10);
		
		if( Gdx.input.isTouched() ) {
			int BOUNDARY = 115;
			int x = Gdx.input.getX();
			int y = Gdx.input.getY();
			
			int height = Gdx.graphics.getHeight();
			
			int capX = x-32; // subtract 32 to get center of image
			
			// note, input goes from (0,0) in top-left... drawing is from (0,0) in bottom-left... so we must subtract from height
			int capY = height-y-32;
			
			// Cap the x,y values so they don't go beyond joystick boundaries
			if( capX > BOUNDARY ) capX = BOUNDARY;
			
			if( capY > BOUNDARY ) capY = BOUNDARY;
			
			// center joystick on finger/cursor
			batch.draw(joystick, capX, capY );
		}
		else {
			// center joystick to bottom-left
			batch.draw(joystick, 10 + 32, 10 + 32);
		}
		
		batch.end();
	}

	@Override
	public void resize(int arg0, int arg1) {
	}

	@Override
	public void resume() {
	}
	
	float degToRad(float degAng) {
		return ((float)(degAng*Math.PI)/(float)ANGLE180);
	}
	
	public void createTables()
  	{
		int i;
	    float radian;
	    sinTable = new float[ANGLE360+1];
	    invSinTable = new float[ANGLE360+1];
	    cosTable = new float[ANGLE360+1];
	    invCosTable = new float[ANGLE360+1];
	    tanTable = new float[ANGLE360+1];
	    invTanTable = new float[ANGLE360+1];
	    fishTable = new float[ANGLE60+1];
	    xStepTable = new float[ANGLE360+1];
	    yStepTable = new float[ANGLE360+1];

	    for (i=0; i<=ANGLE360;i++)
	    {
	      // get the radian value (the last addition is to avoid division by 0, try removing
	          // that and you'll see a hole in the wall when a ray is at 0, 90, 180, or 270 degree)
	      radian = degToRad(i) + (float)(0.0001);
	      sinTable[i]=(float)Math.sin(radian);
	      invSinTable[i]=(1.0f/(sinTable[i]));
	      cosTable[i]=(float)Math.cos(radian);
	      invCosTable[i]=(1.0f/(cosTable[i]));
	      tanTable[i]=(float)Math.tan(radian);
	      invTanTable[i]=(1.0f/tanTable[i]);

	      //  you can see that the distance between xi is the same
	      //  if we know the angle
	      //  _____|_/next xi______________
	      //       |
	      //  ____/|next xi_________   slope = tan = height / dist between xi's
	      //     / |
	      //  __/__|_________  dist between xi = height/tan where height=tile size
	      // old xi|
	      //                  distance between xi = x_step[view_angle];
	      //
	      // facing left
	      if (i>=ANGLE90 && i<ANGLE270)
	      {
	        xStepTable[i] = (float)(TILE_SIZE/tanTable[i]);
	        if (xStepTable[i]>0)
	          xStepTable[i]=-xStepTable[i];
	      }
	      // facing right
	      else
	      {
	        xStepTable[i] = (float)(TILE_SIZE/tanTable[i]);
	        if (xStepTable[i]<0)
	          xStepTable[i]=-xStepTable[i];
	      }

	      // FACING DOWN
	      if (i>=ANGLE0 && i<ANGLE180)
	      {
	        yStepTable[i] = (float)(TILE_SIZE*tanTable[i]);
	        if (yStepTable[i]<0)
	          yStepTable[i]=-yStepTable[i];
	      }
	      // FACING UP
	      else
	      {
	        yStepTable[i] = (float)(TILE_SIZE*tanTable[i]);
	        if (yStepTable[i]>0)
	          yStepTable[i]=-yStepTable[i];
	      }
	    }

	    for (i=-ANGLE30; i<=ANGLE30; i++)
	    {
	        radian = degToRad(i);
	        // we don't have negative angle, so make it start at 0
	        // this will give range 0 to 320
	        fishTable[i+ANGLE30] = (float)(1.0f/Math.cos(radian));
	    }

        // CREATE A SIMPLE MAP
        byte[] map=
        {
                W,W,W,W,W,W,W,W,W,W,W,W,
                W,O,O,O,O,O,O,O,O,O,O,W,
                W,O,O,O,O,O,O,O,O,O,O,W,
                W,O,O,O,O,O,O,O,O,O,O,W,
                W,O,O,O,O,O,O,O,O,O,O,W,
                W,O,O,O,O,O,O,O,W,O,O,W,
                W,O,O,O,O,O,O,O,O,O,O,W,
                W,O,O,O,O,O,O,O,O,O,O,W,
                W,O,O,O,O,O,O,O,O,O,O,W,
                W,O,O,O,O,O,O,O,O,O,O,W,
                W,O,O,O,O,O,O,O,O,O,O,W,
                W,W,W,W,W,W,W,W,W,W,W,W
        };
        fMap=map;
	}
	
	public void drawBackground() {
		// sky
		int c=25;
		int r;
		for( r = 0; r < PROJECTION_PLANE_HEIGHT/2; r += 10 ) {
			pixmap.setColor(1.0f, 0.5f, 1.0f, 1.0f);
			pixmap.fillRectangle(0, r, PROJECTION_PLANE_WIDTH, 10);
			c += 20;
		}
		
		// ground
		c = 22;
		for( ; r < PROJECTION_PLANE_HEIGHT; r += 15 ) {
			pixmap.setColor( (float)c / (float)255, 0.1f, 0.1f, 1.0f );
			pixmap.fillRectangle(0, r, PROJECTION_PLANE_WIDTH, 15);
			c += 15;
		}
	}
	
	public void castRays() {		
	    int verticalGrid;        // horizotal or vertical coordinate of intersection
	    int horizontalGrid;      // theoritically, this will be multiple of TILE_SIZE
	                             // , but some trick did here might cause
	                             // the values off by 1
	    
	    int distToNextVerticalGrid; // how far to the next bound (this is multiple of
	    int distToNextHorizontalGrid; // tile size)
	    float xIntersection;  // x and y intersections
	    float yIntersection;
	    float distToNextXIntersection;
	    float distToNextYIntersection;

	    int xGridIndex;        // the current cell that the ray is in
	    int yGridIndex;

	    float distToVerticalGridBeingHit;      // the distance of the x and y ray intersections from
	    float distToHorizontalGridBeingHit;      // the viewpoint

	    int castArc, castColumn;

	    castArc = playerAngle;
	        // field of view is 60 degree with the point of view (player's direction in the middle)
	        // 30  30
	        //    ^
	        //  \ | /
	        //   \|/
	        //    v
	        // we will trace the rays starting from the leftmost ray
	        castArc-=ANGLE30;
	        // wrap around if necessary
	        
	    if (castArc < 0)
	    {
	      castArc=ANGLE360 + castArc;
	    }

	    for (castColumn=0; castColumn < PROJECTION_PLANE_WIDTH; castColumn+=5)
	    {
	          // ray is between 0 to 180 degree (1st and 2nd quadrant)
	          // ray is facing down
	      if (castArc > ANGLE0 && castArc < ANGLE180)
	      {
	                // truncuate then add to get the coordinate of the FIRST grid (horizontal
	                // wall) that is in front of the player (this is in pixel unit)
	                // ROUND DOWN
	        horizontalGrid = (playerY/TILE_SIZE)*TILE_SIZE  + TILE_SIZE;

	        // compute distance to the next horizontal wall
	        distToNextHorizontalGrid = TILE_SIZE;

	        float xtemp = invTanTable[castArc]*(horizontalGrid-playerY);
	                // we can get the vertical distance to that wall by
	                // (horizontalGrid-GLplayerY)
	                // we can get the horizontal distance to that wall by
	                // 1/tan(arc)*verticalDistance
	                // find the x interception to that wall
	        xIntersection = xtemp + playerX;
	      }
	      // else, the ray is facing up
	      else
	      {
	        horizontalGrid = (playerY/TILE_SIZE)*TILE_SIZE;
	        distToNextHorizontalGrid = -TILE_SIZE;

	        float xtemp = invTanTable[castArc]*(horizontalGrid - playerY);
	        xIntersection = xtemp + playerX;

	        horizontalGrid--;
	      }
	          // LOOK FOR HORIZONTAL WALL
	      if (castArc==ANGLE0 || castArc==ANGLE180)
	      {
	        distToHorizontalGridBeingHit=9999999F;//Float.MAX_VALUE;
	      }
	      // else, move the ray until it hits a horizontal wall
	      else
	      {
	        distToNextXIntersection = xStepTable[castArc];
	        while (true)
	        {
	          xGridIndex = (int)(xIntersection/TILE_SIZE);
	          // in the picture, yGridIndex will be 1
	          yGridIndex = (horizontalGrid/TILE_SIZE);

	          if ((xGridIndex>=MAP_WIDTH) ||
	            (yGridIndex>=MAP_HEIGHT) ||
	            xGridIndex<0 || yGridIndex<0)
	          {
	            distToHorizontalGridBeingHit = Float.MAX_VALUE;
	            break;
	          }
	          else if ((fMap[yGridIndex*MAP_WIDTH+xGridIndex])!=O)
	          {
	            distToHorizontalGridBeingHit  = (xIntersection-playerX)*invCosTable[castArc];
	            break;
	          }
	          // else, the ray is not blocked, extend to the next block
	          else
	          {
	            xIntersection += distToNextXIntersection;
	            horizontalGrid += distToNextHorizontalGrid;
	          }
	        }
	      }


	      // FOLLOW X RAY
	      if (castArc < ANGLE90 || castArc > ANGLE270)
	      {
	        verticalGrid = TILE_SIZE + (playerX/TILE_SIZE)*TILE_SIZE;
	        distToNextVerticalGrid = TILE_SIZE;

	        float ytemp = tanTable[castArc]*(verticalGrid - playerX);
	        yIntersection = ytemp + playerY;
	      }
	      // RAY FACING LEFT
	      else
	      {
	        verticalGrid = (playerX/TILE_SIZE)*TILE_SIZE;
	        distToNextVerticalGrid = -TILE_SIZE;

	        float ytemp = tanTable[castArc]*(verticalGrid - playerX);
	        yIntersection = ytemp + playerY;

	        verticalGrid--;
	      }
	          // LOOK FOR VERTICAL WALL
	      if (castArc==ANGLE90||castArc==ANGLE270)
	      {
	        distToVerticalGridBeingHit = 9999999;//Float.MAX_VALUE;
	      }
	      else
	      {
	        distToNextYIntersection = yStepTable[castArc];
	        while (true)
	        {
	          // compute current map position to inspect
	          xGridIndex = (verticalGrid/TILE_SIZE);
	          yGridIndex = (int)(yIntersection/TILE_SIZE);

	          if ((xGridIndex>=MAP_WIDTH) ||
	            (yGridIndex>=MAP_HEIGHT) ||
	            xGridIndex<0 || yGridIndex<0)
	          {
	            distToVerticalGridBeingHit = Float.MAX_VALUE;
	            break;
	          }
	          else if ((fMap[yGridIndex*MAP_WIDTH+xGridIndex])!=O)
	          {
	            distToVerticalGridBeingHit =(yIntersection-playerY)*invSinTable[castArc];
	            break;
	          }
	          else
	          {
	            yIntersection += distToNextYIntersection;
	            verticalGrid += distToNextVerticalGrid;
	          }
	        }
	      }

	      // DRAW THE WALL SLICE
	      float dist;
	      int topOfWall;   // used to compute the top and bottom of the sliver that
	      int bottomOfWall;   // will be the staring point of floor and ceiling
	      // determine which ray strikes a closer wall.
	      // if yray distance to the wall is closer, the yDistance will be shorter than
	          // the xDistance
	      if (distToHorizontalGridBeingHit < distToVerticalGridBeingHit)
	          {
	                // the next function call (drawRayOnMap()) is not a part of raycating rendering part, 
	                // it just draws the ray on the overhead map to illustrate the raycasting process
	            //drawRayOnOverheadMap(xIntersection, horizontalGrid);
	    	  dist=distToHorizontalGridBeingHit;
	    	  pixmap.setColor(0.5f, 0.5f, 0.5f, 1.0f);
	      }
	      // else, we use xray instead (meaning the vertical wall is closer than
	      //   the horizontal wall)
	          else
	          {
	                // the next function call (drawRayOnMap()) is not a part of raycating rendering part, 
	                // it just draws the ray on the overhead map to illustrate the raycasting process
	            //drawRayOnOverheadMap(verticalGrid, yIntersection);
	        dist=distToVerticalGridBeingHit;
	        pixmap.setColor(0.25f, 0.25f, 0.25f, 1.0f);
	          }

	          // correct distance (compensate for the fishbown effect)
	      dist /= fishTable[castColumn];
	          // projected_wall_height/wall_height = fPlayerDistToProjectionPlane/dist;
	          int projectedWallHeight=(int)(WALL_HEIGHT*(float)playerDistanceToProjectionPlane/dist);
	      bottomOfWall = projectionPlaneYCenter+(int)(projectedWallHeight*0.5F);
	      topOfWall = PROJECTION_PLANE_HEIGHT-bottomOfWall;
	          if (bottomOfWall>=PROJECTION_PLANE_HEIGHT)
	                bottomOfWall=PROJECTION_PLANE_HEIGHT-1;
	          //fOffscreenGraphics.drawLine(castColumn, topOfWall, castColumn, bottomOfWall);
	          //fOffscreenGraphics.fillRect(castColumn, topOfWall, 5, projectedWallHeight);
	          pixmap.fillRectangle(castColumn, topOfWall, 5, projectedWallHeight);
	          
	          // TRACE THE NEXT RAY
	          castArc+=5;
	          if (castArc>=ANGLE360)
	                castArc-=ANGLE360;
	    }
	}
	

}
