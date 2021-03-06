// surfCLR.h

#pragma once

#pragma unmanaged
#include <cv.h>
#include <highgui.h>
#include <vfw.h>




		
		inline float rgb2y(int r, int g, int b) 
		{
				__declspec(align(16)) const float Y[4] = { 0.2989f, 0.5870f, 0.1140f, 0.0f };
				__declspec(align(16)) float rgb[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

				rgb[0] = (float)r;
				rgb[1] = (float)g;
				rgb[2] = (float)b;

				float out[1] = { 0.0f };

				__m128 n = _mm_load_ps(Y);
				__m128 m = _mm_load_ps(rgb);

				__m128 res = _mm_mul_ps(m, n);

				//res = a,b,c,d
				m = _mm_movelh_ps(res, res);     //a,b,a,b
				n = _mm_movehl_ps(res, res);     //c,d,c,d
				m = _mm_add_ps(m, n);            //r0,r1,r0,r1
				n = _mm_shuffle_ps(m, m, _MM_SHUFFLE(2, 3, 0, 1)); //r1,r0,r1,r0
				res = _mm_add_ss(m, n);               //r1+r0

				_mm_store_ss(out, res);
				return out[0];
		}





#pragma managed

using namespace System;
using namespace System::Drawing;
using namespace System::Drawing::Imaging;

using namespace Microsoft::Xna::Framework::Graphics;

using namespace System::Runtime::InteropServices;


#include <msclr/marshal_cppstd.h>

namespace surfCLR {

	public ref class SURF
	{
		// TODO: Add your methods for this class here.
	private:



		cv::SURF2 * _NativeSURF2;
		CvCapture* _capture;


		GraphicsDevice ^ _graphics;

		Texture2D ^ _imgTex;
		IplImage* _imgBuffer ; 


		Drawing::Bitmap ^ _bmp ;

	public: 

		SURF()
		{
			_NativeSURF2 = NULL;
			_capture = NULL;
			_imgBuffer = NULL;
			
		}

		~SURF()
		{

			this->!SURF();

		}

		!SURF()
		{

			if( _capture != NULL)
			{
				pin_ptr<CvCapture*> p = &_capture;
				cvReleaseCapture( p );
				_capture = NULL;
			}

			if( _NativeSURF2 != NULL)
			{
				delete _NativeSURF2;
				_NativeSURF2 = NULL;
			}

		}


		double GetExtremaTime(){return _NativeSURF2->_extremaTime;}
		double GetDescriptorTime(){return _NativeSURF2->_descriptorTime;}
		double GetCompareTime(){return _NativeSURF2->_compareTime;}

		int GetCX(){return _NativeSURF2->_CX;}
		int GetCY(){return _NativeSURF2->_CY;}
		float GetCS(){return _NativeSURF2->_CS;}
		//double GetExtremaTime(){return _NativeSURF2->_extremaTime;}


		Drawing::Bitmap^ imgToBitmap(IplImage* img)
		{
		
				PixelFormat pf;
				switch(img->nChannels)
				{
                case 1:
					{
					pf = PixelFormat::Format8bppIndexed; 

					//dst.Palette = plt;
			

					Drawing::Bitmap^ bmp = gcnew Drawing::Bitmap(img->width,img->height,img->widthStep, pf,IntPtr( img->imageData));
					//bmp->Palette = plt;

					   ColorPalette^ plt = bmp->Palette;
					for (int x = 0; x < 256; x++)
					{
						plt->Entries[x] = System::Drawing::Color::FromArgb(x, x, x);
					}
					
					bmp->Palette = plt;

					return bmp;
					}
					
					
                case 3:
                    pf = PixelFormat::Format24bppRgb; 
					return gcnew Drawing::Bitmap(img->width,img->height,img->widthStep, pf,IntPtr( img->imageData));
					
                case 4:
                    pf = PixelFormat::Format32bppArgb; 
					return gcnew Drawing::Bitmap(img->width,img->height,img->widthStep, pf,IntPtr( img->imageData));
					
                default:
					break;

				}

			return nullptr;		
		}


	
		
		Texture2D^ imgToTexture(IplImage* img)
		{
		
				PixelFormat pf;
				switch(img->nChannels)
				{
                case 1:
					{
					pf = PixelFormat::Format8bppIndexed; 

					//dst.Palette = plt;
			
					//Texture3D^ bmp22 = gcnew Texture3D(_graphics,img->width,img->height,1, 1,TextureUsage::None,SurfaceFormat::Depth24);

					Texture2D^ bmp = gcnew Texture2D(_graphics,img->width,img->height,1, TextureUsage::None,SurfaceFormat::Color);
					cli::array<Microsoft::Xna::Framework::Graphics::Color>^ dest = gcnew cli::array<Microsoft::Xna::Framework::Graphics::Color>(img->width * img->height);

					//Microsoft::Xna::Framework::Graphics::Color[] dest = gcnew Microsoft::Xna::Framework::Graphics::Color[img->width * img->height];

					//cli::array<Microsoft::Xna::Framework::Graphics::Color> ^ dest = gcnew cli::array<Microsoft::Xna::Framework::Graphics::Color>[img->width * img->height];
					//bmp->GetData<Microsoft::Xna::Framework::Graphics::Color>(dest);

					char * pImg = img->imageData;

					int index = 0;
					for(int h = 0 ; h < img->height;h++)
					{
						for(int w = 0; w < img->width;w++)
						{
							dest[index].R = *pImg;
							dest[index].G = *pImg;
							dest[index].B = *pImg;
							dest[index].A = 255;
							

							index++;
							pImg++;
						}
						pImg += (img->widthStep - img->width);
					}
				
					bmp->SetData(dest);

					//bmp->Palette = plt;

					//   ColorPalette^ plt = bmp->Palette;
					//for (int x = 0; x < 256; x++)
					//{
					//	plt->Entries[x] = Color::FromArgb(x, x, x);
					//}
					//
					//bmp->Palette = plt;

					return bmp;
					}
					
					
                case 3:
					
                    pf = PixelFormat::Format24bppRgb; 
					return gcnew Texture2D(_graphics,img->width,img->height,1, TextureUsage::None,SurfaceFormat::Bgr24);
					
                case 4:
                    pf = PixelFormat::Format32bppArgb; 
					return gcnew Texture2D(_graphics,img->width,img->height,1, TextureUsage::None,SurfaceFormat::Rgba32);
					
                default:
					break;

				}

			return nullptr;		
		}


		Drawing::Bitmap^ Init(String ^ fileName)
		{
			msclr::interop::marshal_context c;


			//CvCaptureCAM_VFW


			_capture = cvCaptureFromCAM(0); // capture from video device #0

			
			cvSetCaptureProperty( _capture, CV_CAP_PROP_FRAME_WIDTH, 320 );
			cvSetCaptureProperty( _capture, CV_CAP_PROP_FRAME_HEIGHT, 240 );
			
//			cvSetCaptureProperty( _capture, CV_CAP_PROP_FORMAT, 32 );


			//_capture->setProperty
			//capSetVideoFormat(0,0,0);


			
			
			_NativeSURF2 = new cv::SURF2();
			IplImage* object = cvLoadImage(c.marshal_as<const char *>(fileName), CV_LOAD_IMAGE_GRAYSCALE );
//			_NativeSURF2->InitResource();
			_NativeSURF2->Init(object);

			
			return imgToBitmap( object);

		}

		
		Texture2D^ InitToTexture(GraphicsDevice ^ graphics,String ^ fileName)
		{

			_graphics = graphics;

			msclr::interop::marshal_context c;

			//_capture = cvCaptureFromCAM(0); // capture from video device #0

			
			
			_NativeSURF2 = new cv::SURF2();
			IplImage* object = cvLoadImage(c.marshal_as<const char *>(fileName), CV_LOAD_IMAGE_GRAYSCALE );
//			_NativeSURF2->InitResource();
			_NativeSURF2->Init(object);

			
			return imgToTexture( object);

		}


		//cli::array<Microsoft::Xna::Framework::Graphics::Color>^ _dest;
		//cli::array<unsigned char>^ _dest;
		

		IplImage * imgToCVImage(System::IntPtr^ sPtr,int len)
		{
			IplImage * img = cvCreateImage(cvSize(320,240),IPL_DEPTH_8U,1);

			return NULL;
		}

		cli::array<int> ^ _dest;

		Texture2D ^ UpdateFrameToTexture(System::IntPtr^ sPtr,int len,int w,int h)
		{


			if( _imgBuffer == NULL)
			{
				_imgBuffer = cvCreateImage(cvSize(w,h),IPL_DEPTH_8U,4);
				_imgTex = gcnew Texture2D(_graphics,_imgBuffer->width,_imgBuffer->height,1, TextureUsage::None,SurfaceFormat::Bgr32);
				_dest = gcnew cli::array<int>(_imgBuffer->width * _imgBuffer->height);
			}

			int * src = (int*) sPtr->ToPointer();
			unsigned char * dst = (unsigned char *)_imgBuffer->imageData;


			src += _imgBuffer->width * (_imgBuffer->height -1);

			for(int h = 0; h < _imgBuffer->height;h++)
			{	
				//for(int w = 0 ; w < _imgBuffer->width;w++)
				{
					memcpy(dst,src,_imgBuffer->width*4);
					
				}


				src -= _imgBuffer->width;
				dst += (_imgBuffer->widthStep);// - _imgBuffer->width);
				
			}
			
				
			
			 _NativeSURF2->Compare32(_imgBuffer) ;



			Marshal::Copy(System::IntPtr(_imgBuffer->imageData),_dest,0,_imgBuffer->width * _imgBuffer->height);
			
			_imgTex->SetData(_dest);


			return _imgTex;
		}

		

		Drawing::Bitmap ^ UpdateFrame()
		{
			IplImage* img = 0; 


			if(!cvGrabFrame(_capture)){              // capture a frame 
			  
				return nullptr;
			  
			}

			img=cvRetrieveFrame(_capture);           // retrieve the captured frame

			_NativeSURF2->Compare(img);

			if(_bmp == nullptr)
			{

				_bmp = imgToBitmap(img);
			}else
			{
				
				BitmapData^ bd = _bmp->LockBits(System::Drawing::Rectangle(0, 0,_bmp->Width, _bmp->Height), ImageLockMode::WriteOnly, _bmp->PixelFormat);
				
				if( bd->Stride == img->widthStep)
					memcpy_s(bd->Scan0.ToPointer(),bd->Stride * bd->Height, img->imageData,img->widthStep * img->height);

				_bmp->UnlockBits(bd);

			

			}


			return _bmp;
		}


	};
}
