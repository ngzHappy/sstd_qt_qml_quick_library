#define _SSTD_HAS_GLEW_0_1 (1u)

#include "GLSSTD/glew.h"
#include "sstd_glew_utility.hpp"
#include <cassert>
#include <tuple>
#include <utility>
#include <array>
#include <string_view>
using namespace std::string_view_literals;
#include <QtCore/qdebug.h>

namespace sstd::opengl_utility {

    inline constexpr void checkQImageFormat() {
        static_assert ((defaultQImageFormat() == QImage::Format_RGBA8888) ||
            (defaultQImageFormat() == QImage::Format_RGBA64));
    }

    /*获得texture的size*/
    SSTD_QT_SYMBOL_DECL QSize getTextureSize(GLuint arg) {

        if (arg < 1) {
            return{ 0,0 };
        }

        GLint width{ 0 };
        GLint height{ 0 };
        glGetTextureLevelParameteriv(arg, 0, GL_TEXTURE_WIDTH, &width);
        glGetTextureLevelParameteriv(arg, 0, GL_TEXTURE_HEIGHT, &height);

        return{ width,height };

    }

    /*创建新的texture*/
    SSTD_QT_SYMBOL_DECL GLuint createTexture(QImage arg) {

        const auto varImage =
            arg.convertToFormat(defaultQImageFormat());

        if (varImage.isNull()) {
            return 0;
        }

        GLuint varAns{ 0 };

        glCreateTextures(GL_TEXTURE_2D, 1, &varAns);

        if (varAns < 1) {
            return 0;
        }

        if constexpr (defaultQImageFormat() == QImage::Format_RGBA64) {
            glTextureStorage2D(varAns, 1, GL_RGBA16,
                varImage.width(), varImage.height());

            glTextureSubImage2D(varAns, 0,
                0, 0,
                varImage.width(), varImage.height(),
                GL_RGBA, GL_UNSIGNED_SHORT,
                varImage.constBits());
        } else if constexpr (defaultQImageFormat() == QImage::Format_RGBA8888) {
            glTextureStorage2D(varAns, 1, GL_RGBA8,
                varImage.width(), varImage.height());

            glTextureSubImage2D(varAns, 0,
                0, 0,
                varImage.width(), varImage.height(),
                GL_RGBA, GL_UNSIGNED_BYTE,
                varImage.constBits());
        } else {
            checkQImageFormat();
        }

        glTextureParameteri(varAns, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTextureParameteri(varAns, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

        return varAns;
    }

    /*更新一个texture*/
    SSTD_QT_SYMBOL_DECL void updateTexture(GLuint * argTextureID, QImage argImage) {

        assert(argTextureID);

        if (*argTextureID < 1) {
            /*创建一个新的texture...*/
            *argTextureID = createTexture(argImage);
            return;
        }

        auto varImageSize = argImage.size();
        auto varTextureSize = getTextureSize(*argTextureID);

        if (varImageSize != varTextureSize) {
            /*如果texture大小改变，删除旧的texture并创建新的*/
            glDeleteTextures(1, argTextureID);
            *argTextureID = createTexture(argImage);
            return;
        }

        /*更新已经存在的texture*/
        const auto varImage =
            argImage.convertToFormat(defaultQImageFormat());

        if constexpr (defaultQImageFormat() == QImage::Format_RGBA64) {
            glTextureSubImage2D(*argTextureID, 0,
                0, 0,
                varImage.width(), varImage.height(),
                GL_RGBA, GL_UNSIGNED_SHORT,
                varImage.constBits());
        } else if constexpr (defaultQImageFormat() == QImage::Format_RGBA8888) {
            glTextureSubImage2D(*argTextureID, 0,
                0, 0,
                varImage.width(), varImage.height(),
                GL_RGBA, GL_UNSIGNED_BYTE,
                varImage.constBits());
        } else {
            checkQImageFormat();
        }

    }

    namespace {
        using gl_program_type = std::pair<std::string, GLenum>;
    }/*namespace*/

    inline static bool _1_createAShader(const gl_program_type & arg, GLuint * ans) {

        *ans = glCreateShader(std::get<1>(arg));

        if (0 == (*ans)) {
            qWarning("create shader false ");
            return false;
        }

        return true;
    }

    template<std::size_t argC>
    inline static GLuint _1_createVFProgram(
        const std::array<gl_program_type, argC> & args) {

        for (const auto & varI : args) {
            if (std::get<0>(varI).empty()) {
                return 0;
            }
        }

        class ShaderFree {
        public:
            std::array< GLuint, argC > data;
            inline ShaderFree() {
                for (auto & varI : data) {
                    varI = 0;
                }
            }
            inline ~ShaderFree() {
                for (auto & varI : data) {
                    glDeleteShader(varI);
                }
            }
        } shaders;

        {/*1*/
            std::size_t varJ = 0;
            for (const auto & varI : args) {
                if (false == _1_createAShader(varI, &(shaders.data[varJ]))) {
                    return 0;
                }
                ++varJ;
            }
        }

        {/*2*/
            const GLchar * sources[argC];
            GLint lengths[argC];

            std::size_t varJ = 0;
            for (const auto & varI : args) {
                sources[varJ] = std::get<0>(varI).data();
                lengths[varJ] = static_cast<GLint>(
                    std::get<0>(varI).size());
                ++varJ;
            }

            varJ = 0;
            for (; varJ < argC; ++varJ) {
                glShaderSource(shaders.data[varJ], 1, &sources[varJ], &lengths[varJ]);
                glCompileShader(shaders.data[varJ]);
                GLint testVal{ false };
                glGetShaderiv(shaders.data[varJ], GL_COMPILE_STATUS, &testVal);
                if (testVal == GL_FALSE) {
                    GLint log_length;
                    glGetShaderiv(shaders.data[varJ], GL_INFO_LOG_LENGTH, &log_length);
                    log_length += 16;
                    sstd::detail::utf8String infos_;
                    infos_.resize(log_length, char(0));
                    char * info = infos_.data();
                    glGetShaderInfoLog(shaders.data[varJ], log_length, nullptr, info);
                    qWarning() << QString::fromUtf8(info);
                    return 0;
                }
            }

        }

        GLuint program = glCreateProgram();
        if (0 == program) {
            qWarning("create program error ");
            return 0;
        }

        for (const auto & varI : shaders.data) {
            glAttachShader(program, varI);
        }

        glLinkProgram(program);

        {
            GLint testVal{ false };
            glGetProgramiv(program, GL_LINK_STATUS, &testVal);
            if (testVal == GL_FALSE) {
                GLint log_length;
                glGetProgramiv(program, GL_INFO_LOG_LENGTH, &log_length);
                log_length += 16;
                sstd::detail::utf8String infos_;
                infos_.resize(log_length, char(0));
                char * info = infos_.data();
                glGetProgramInfoLog(program, log_length, nullptr, info);
                qWarning() << QString::fromUtf8(info);
                glDeleteProgram(program);
                return 0;
            }
        }

        return program;
    }

    template<typename ... Args>
    inline static GLuint _0_createVFProgram(Args && ... args) {
        const std::array<gl_program_type, sizeof...(Args)>
            varArg{ std::forward<Args>(args)... };
        return _1_createVFProgram<sizeof...(Args)>(varArg);
    }

    SSTD_QT_SYMBOL_DECL GLuint createVFProgram(
        std::string_view argV,
        std::string_view argF) {

        return _0_createVFProgram(
            gl_program_type{ argV, GL_VERTEX_SHADER },
            gl_program_type{ argF, GL_FRAGMENT_SHADER });

    }

}/*namespace sstd*/





//
// https://www.khronos.org/registry/OpenGL-Refpages/gl4/
//
