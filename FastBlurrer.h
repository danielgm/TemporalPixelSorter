#pragma once

class FastBlurrer {
  public:
    FastBlurrer() {
      initialized = false;
    }

    void init(int _width, int _radius) {
      if (initialized) {
        destroy();
      }

      w = _width;
      radius = _radius;

      wm = w-1;
      hm = 0;
      div = radius+radius+1;
      r = new int[w];
      g = new int[w];
      b = new int[w];
      rsum,gsum,bsum,x,y,i,p,p1,p2,yp,yi,yw;
      vmin = new int[w];
      vmax = new int[w];
      dv = new int[256*div];

      initialized = true;
    }

    ~FastBlurrer() {
      if (initialized) {
        destroy();
      }
    }

    /**
     * Thanks Mario!
     * @see http://incubator.quasimondo.com/processing/superfastblur.pde
     */
    void blur(unsigned char* pixels) {
      if (!initialized || radius < 1) return;

      for (i=0;i<256*div;i++){
        dv[i]=(i/div);
      }

      yi=0;

      rsum=gsum=bsum=0;
      for(i=-radius;i<=radius;i++){
        p=pixels[yi+min(wm,max(i,0))];
        rsum+=(p & 0xff0000)>>16;
        gsum+=(p & 0x00ff00)>>8;
        bsum+= p & 0x0000ff;
      }
      for (x=0;x<w;x++){
        r[yi]=dv[rsum];
        g[yi]=dv[gsum];
        b[yi]=dv[bsum];

        vmin[x]=min(x+radius+1,wm);
        vmax[x]=max(x-radius,0);
        p1=pixels[vmin[x]];
        p2=pixels[vmax[x]];

        rsum+=((p1 & 0xff0000)-(p2 & 0xff0000))>>16;
        gsum+=((p1 & 0x00ff00)-(p2 & 0x00ff00))>>8;
        bsum+= (p1 & 0x0000ff)-(p2 & 0x0000ff);
        yi++;
      }

      for (x=0;x<w;x++){
        rsum=gsum=bsum=0;
        yp=-radius*w;
        for(i=-radius;i<=radius;i++){
          yi=max(0,yp)+x;
          rsum+=r[yi];
          gsum+=g[yi];
          bsum+=b[yi];
          yp+=w;
        }
        yi=x;

        pixels[yi]=0xff000000 | (dv[rsum]<<16) | (dv[gsum]<<8) | dv[bsum];
        if(x==0){
          vmin[0]=0;
          vmax[0]=max(-radius,0)*w;
        }
        p1=x+vmin[0];
        p2=x+vmax[0];

        rsum+=r[p1]-r[p2];
        gsum+=g[p1]-g[p2];
        bsum+=b[p1]-b[p2];
      }
    }

  private:
    bool initialized;

    int w;
    int radius;

    int div;
    int* r;
    int* g;
    int* b;
    int rsum,gsum,bsum,x,y,i,p,p1,p2,yp,yi,yw;
    int* vmin;
    int* vmax;
    int* dv;

    void destroy() {
      delete[] r;
      delete[] g;
      delete[] b;
      delete[] vmin;
      delete[] vmax;
      delete[] dv;
    }
};

#endif
