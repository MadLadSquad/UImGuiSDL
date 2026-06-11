#ifdef __APPLE__
#import <AppKit/AppKit.h>
#import <QuartzCore/CAMetalLayer.h>

extern "C" void uimgui_sdl_update_macos_layer(void* nswin_ptr)
{
    NSWindow* nswin = static_cast<NSWindow*>(nswin_ptr);
    if (!nswin) return;

    NSView* contentView = nswin.contentView;
    if (!contentView) return;

    CALayer* layer = contentView.layer;
    if (layer && [layer isKindOfClass:[CAMetalLayer class]])
    {
        CAMetalLayer* metalLayer = (CAMetalLayer*)layer;
        
        const CGFloat scale = nswin.backingScaleFactor;
        const CGRect viewBounds = contentView.bounds;
        const int width  = static_cast<int>(viewBounds.size.width  * scale);
        const int height = static_cast<int>(viewBounds.size.height * scale);

        metalLayer.frame = viewBounds;
        metalLayer.contentsScale = scale;
        metalLayer.drawableSize = CGSizeMake(width, height);
    }
}
#endif
