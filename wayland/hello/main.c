#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <wayland-client.h>
#include <wayland-egl.h>
#include <EGL/egl.h>
#include <GLES2/gl2.h>

// XDG Shell协议头文件（需要用wayland-scanner生成）
#include "xdg-shell-client-protocol.h"

struct app_state {
    struct wl_display *display;
    struct wl_registry *registry;
    struct wl_compositor *compositor;
    struct wl_surface *surface;
    struct xdg_wm_base *xdg_wm_base;
    struct xdg_surface *xdg_surface;
    struct xdg_toplevel *xdg_toplevel;
    
    struct wl_egl_window *egl_window;
    EGLDisplay egl_display;
    EGLContext egl_context;
    EGLSurface egl_surface;
    
    int width, height;
    int running;
};

// XDG WM Base事件处理
static void xdg_wm_base_ping(void *data, struct xdg_wm_base *xdg_wm_base, uint32_t serial) {
    xdg_wm_base_pong(xdg_wm_base, serial);
}

static const struct xdg_wm_base_listener xdg_wm_base_listener = {
    .ping = xdg_wm_base_ping,
};

// XDG Surface事件处理
static void xdg_surface_configure(void *data, struct xdg_surface *xdg_surface, uint32_t serial) {
    struct app_state *state = data;
    xdg_surface_ack_configure(xdg_surface, serial);
    
    // 首次配置后开始渲染
    wl_surface_commit(state->surface);
}

static const struct xdg_surface_listener xdg_surface_listener = {
    .configure = xdg_surface_configure,
};

// XDG Toplevel事件处理
static void xdg_toplevel_configure(void *data, struct xdg_toplevel *xdg_toplevel,
                                   int32_t width, int32_t height, struct wl_array *states) {
    struct app_state *state = data;
    if (width > 0 && height > 0) {
        state->width = width;
        state->height = height;
        if (state->egl_window) {
            wl_egl_window_resize(state->egl_window, width, height, 0, 0);
        }
    }
}

static void xdg_toplevel_close(void *data, struct xdg_toplevel *xdg_toplevel) {
    struct app_state *state = data;
    state->running = 0;
}

static const struct xdg_toplevel_listener xdg_toplevel_listener = {
    .configure = xdg_toplevel_configure,
    .close = xdg_toplevel_close,
};

// Registry事件处理 - 发现全局对象
static void registry_global(void *data, struct wl_registry *registry,
                           uint32_t name, const char *interface, uint32_t version) {
    struct app_state *state = data;
    
    if (strcmp(interface, wl_compositor_interface.name) == 0) {
        state->compositor = wl_registry_bind(registry, name, &wl_compositor_interface, 4);
    } else if (strcmp(interface, xdg_wm_base_interface.name) == 0) {
        state->xdg_wm_base = wl_registry_bind(registry, name, &xdg_wm_base_interface, 1);
        xdg_wm_base_add_listener(state->xdg_wm_base, &xdg_wm_base_listener, state);
    }
}

static void registry_global_remove(void *data, struct wl_registry *registry, uint32_t name) {
    // 处理全局对象移除
}

static const struct wl_registry_listener registry_listener = {
    .global = registry_global,
    .global_remove = registry_global_remove,
};

// 初始化EGL
static int init_egl(struct app_state *state) {
    EGLint major, minor, count, n, size;
    EGLConfig *configs;
    EGLBoolean ret;
    
    static const EGLint config_attribs[] = {
        EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
        EGL_RED_SIZE, 8,
        EGL_GREEN_SIZE, 8,
        EGL_BLUE_SIZE, 8,
        EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
        EGL_NONE
    };
    
    static const EGLint context_attribs[] = {
        EGL_CONTEXT_CLIENT_VERSION, 2,
        EGL_NONE
    };
    
    state->egl_display = eglGetDisplay((EGLNativeDisplayType)state->display);
    if (state->egl_display == EGL_NO_DISPLAY) {
        fprintf(stderr, "无法获取EGL display\n");
        return -1;
    }
    
    if (!eglInitialize(state->egl_display, &major, &minor)) {
        fprintf(stderr, "无法初始化EGL\n");
        return -1;
    }
    
    printf("EGL版本: %d.%d\n", major, minor);
    
    eglGetConfigs(state->egl_display, NULL, 0, &count);
    configs = calloc(count, sizeof *configs);
    
    ret = eglChooseConfig(state->egl_display, config_attribs, configs, count, &n);
    if (!ret || n == 0) {
        fprintf(stderr, "无法选择EGL配置\n");
        free(configs);
        return -1;
    }
    
    EGLConfig config = configs[0];
    free(configs);
    
    state->egl_context = eglCreateContext(state->egl_display, config, 
                                         EGL_NO_CONTEXT, context_attribs);
    if (state->egl_context == EGL_NO_CONTEXT) {
        fprintf(stderr, "无法创建EGL context\n");
        return -1;
    }
    
    state->egl_window = wl_egl_window_create(state->surface, state->width, state->height);
    state->egl_surface = eglCreateWindowSurface(state->egl_display, config,
                                               (EGLNativeWindowType)state->egl_window, NULL);
    
    eglMakeCurrent(state->egl_display, state->egl_surface, state->egl_surface, state->egl_context);
    
    return 0;
}

// 渲染函数
static void render(struct app_state *state) {
    static float color = 0.0f;
    
    // 改变背景颜色（简单动画）
    glClearColor(color, 0.2f, 0.5f, 0.5f); // Alpha通道为0.5 半透明, 1.0为不透明
    glClear(GL_COLOR_BUFFER_BIT);
    
    color += 0.01f;
    if (color > 1.0f) color = 0.0f;
    
    // 交换缓冲区
    eglSwapBuffers(state->egl_display, state->egl_surface);
}

int main(int argc, char *argv[]) {
    struct app_state state = {0};
    state.width = 800;
    state.height = 600;
    state.running = 1;
    
    // 1. 连接到Wayland display
    state.display = wl_display_connect(NULL);
    if (!state.display) {
        fprintf(stderr, "无法连接到Wayland display\n");
        return 1;
    }
    printf("已连接到Wayland display\n");
    
    // 2. 获取registry并绑定全局对象
    state.registry = wl_display_get_registry(state.display);
    wl_registry_add_listener(state.registry, &registry_listener, &state);
    
    wl_display_roundtrip(state.display);
    
    if (!state.compositor || !state.xdg_wm_base) {
        fprintf(stderr, "未找到必需的Wayland接口\n");
        return 1;
    }
    
    // 3. 创建surface
    state.surface = wl_compositor_create_surface(state.compositor);
    
    // 4. 创建XDG surface和toplevel（窗口）
    state.xdg_surface = xdg_wm_base_get_xdg_surface(state.xdg_wm_base, state.surface);
    xdg_surface_add_listener(state.xdg_surface, &xdg_surface_listener, &state);
    
    state.xdg_toplevel = xdg_surface_get_toplevel(state.xdg_surface);
    xdg_toplevel_add_listener(state.xdg_toplevel, &xdg_toplevel_listener, &state);
    xdg_toplevel_set_title(state.xdg_toplevel, "Wayland原生窗口");
    
    // 提交surface
    wl_surface_commit(state.surface);
    wl_display_roundtrip(state.display);
    
    // 5. 初始化EGL和OpenGL
    if (init_egl(&state) < 0) {
        return 1;
    }
    
    printf("窗口创建成功，开始渲染循环\n");
    
    // 6. 主事件循环
    while (state.running) {
        wl_display_dispatch_pending(state.display);
        render(&state);
        wl_display_flush(state.display);
    }
    
    // 清理资源
    if (state.xdg_toplevel) xdg_toplevel_destroy(state.xdg_toplevel);
    if (state.xdg_surface) xdg_surface_destroy(state.xdg_surface);
    if (state.surface) wl_surface_destroy(state.surface);
    if (state.xdg_wm_base) xdg_wm_base_destroy(state.xdg_wm_base);
    if (state.compositor) wl_compositor_destroy(state.compositor);
    if (state.registry) wl_registry_destroy(state.registry);
    if (state.display) wl_display_disconnect(state.display);
    
    return 0;
}