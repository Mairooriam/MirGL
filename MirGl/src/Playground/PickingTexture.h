#include <iostream>
namespace Mir{
    class PickingTexture
    {
    private:
        unsigned int m_fbo = 0;
        unsigned int m_pickingTexture = 0;
        unsigned int m_depthTexture = 0;
        // unsigned int m_width = 0;
        // unsigned int m_height = 0;
    public:
        PickingTexture(/* args */);
        ~PickingTexture();
        void Init(unsigned int WindowWidth, unsigned int WindowHeight);
        void EnableWriting();
        void DisableWriting();
    
        struct PixelInfo{
            unsigned int ObjectID = 0;
            unsigned int DrawID = 0;
            unsigned int primID = 0;
            
            void print(){
                std::cout << "ObjectID: " << ObjectID << ", DrawID: " << DrawID << ", PrimID: " << primID << std::endl;
            }
        };
    
        PixelInfo ReadPixel(unsigned int x, unsigned int y);
    };

}
