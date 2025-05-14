set -e

OS=${2}
MODE=${3}
THREADS=${4}

#-Wall -Wextra -Wpedantic -Wconversion
CXX_FLAGS="-std=c++20 -Wall -Wextra -fno-exceptions -Wno-deprecated-declarations -Wno-missing-braces"
CXX_FLAGS_GAME_DEBUG="-g -march=x86-64-v2 -DICHIGO_DEBUG"
CXX_FLAGS_GAME_RELEASE="-g -O3 -march=x86-64-v2"
CXX_FLAGS_IMGUI="-g -march=x86-64-v2"
CXX_FLAGS_BANA="-g -march=x86-64-v2"
CXX_FILES_WIN32=(win32_ichigo.cpp)
CXX_FILES_LINUX=(linux_ichigo.cpp)

CXX_FILES_BANA_WIN32=(bana/bana_platform_win32.cpp)
CXX_FILES_BANA_LINUX=(bana/bana_platform_linux.cpp) # TODO
CXX_FILES_BANA_COMMON=(
    bana/bana.cpp
)

CXX_FILES_DEBUG=(
    main.cpp
    win32_ichigo.cpp
    asset.cpp
    mesh.cpp
)

CXX_FILES_RELEASE=(
    main.cpp
    win32_ichigo.cpp
    asset.cpp
    mesh.cpp
)

WIN32_RC_FILE="res.rc"
IMGUI_CXX_FILES=(./thirdparty/imgui/imgui.cpp ./thirdparty/imgui/imgui_draw.cpp ./thirdparty/imgui/imgui_tables.cpp ./thirdparty/imgui/imgui_widgets.cpp ./thirdparty/imgui/imgui_impl_win32.cpp ./thirdparty/imgui/imgui_impl_opengl3.cpp)
IMGUI_LINUX_CXX_FILES=(./thirdparty/imgui/imgui.cpp ./thirdparty/imgui/imgui_draw.cpp ./thirdparty/imgui/imgui_tables.cpp ./thirdparty/imgui/imgui_widgets.cpp ./thirdparty/imgui/imgui_impl_sdl2.cpp ./thirdparty/imgui/imgui_impl_opengl3.cpp)
LIBS_WIN32="user32 -lwinmm -ldsound -ldxguid -lxinput -lgdi32"
LIBS_LINUX="GL -lSDL2"
EXE_NAME="game.exe"
IMGUI_OBJECT_FILES_DIRECTORY="build/imgui"
BANA_OBJECT_FILES_DIRECTORY="build/bana"
INCLUDE="thirdparty/include"

mkdir -p build/objects
mkdir -p $IMGUI_OBJECT_FILES_DIRECTORY
mkdir -p $BANA_OBJECT_FILES_DIRECTORY

LIBS=""
CXX_FILES=()
CXX_FILES_BANA=()

build_bana() {
    rm -f ${BANA_OBJECT_FILES_DIRECTORY}/*.o
    for file in ${CXX_FILES_BANA[*]}; do
        echo [Bana] $file
        clang++ ${file} ${CXX_FLAGS} ${CXX_FLAGS_BANA} -c -o ${BANA_OBJECT_FILES_DIRECTORY}/$(basename ${file}).o &
    done;

    wait $(jobs -p)
}

if [ "$OS" = "linux" ]; then
    LIBS=$LIBS_LINUX
    CXX_FILES_BANA+=("${CXX_FILES_BANA_COMMON[@]}" "${CXX_FILES_BANA_LINUX[@]}")
    if [ "$MODE" = "debug" ]; then
        CXX_FILES+=("${CXX_FILES_LINUX[@]}" "${CXX_FILES_DEBUG[@]}")
    elif [ "$MODE" = "release" ]; then
        CXX_FILES+=("${CXX_FILES_LINUX[@]}" "${CXX_FILES_RELEASE[@]}")
    else
        echo Invalid build mode
        exit 1
    fi
elif [ "$OS" = "win32" ]; then
    LIBS=$LIBS_WIN32
    CXX_FILES_BANA+=("${CXX_FILES_BANA_COMMON[@]}" "${CXX_FILES_BANA_WIN32[@]}")
    if [ "$MODE" = "debug" ]; then
        CXX_FILES+=("${CXX_FILES_WIN32[@]}" "${CXX_FILES_DEBUG[@]}")
    elif [ "$MODE" = "release" ]; then
        CXX_FILES+=("${CXX_FILES_WIN32[@]}" "${CXX_FILES_RELEASE[@]}")
    else
        echo Invalid build mode
        exit 1
    fi
else
    echo Invalid platform
    exit 1
fi


if [ "${1}" = "run" ]; then
    cd build
    ./$EXE_NAME
    exit 0
fi

if [ "${1}" = "build" ]; then
    rm -f build/objects/*.o

    build_bana
    if [ "$OS" = "win32" ]; then
        type ./thirdparty/tools/ctime.exe && ./thirdparty/tools/ctime.exe -begin ./build/timings.ctm
        llvm-rc $WIN32_RC_FILE
        mv "${WIN32_RC_FILE%.*}.res" build/objects/
        if [ "$MODE" = "debug" ]; then
            files_in_flight=0
            for file in ${CXX_FILES[*]}; do
                echo $file
                clang++ ${CXX_FLAGS} ${CXX_FLAGS_GAME_DEBUG} -I ${INCLUDE} ${file} -c -o build/objects/$(basename ${file}).o &
                (( ++files_in_flight ));
                if (( $files_in_flight == $THREADS )); then
                    echo "Waiting for jobs to finish"
                    wait $(jobs -p)
                    files_in_flight=0
                fi
            done

            wait $(jobs -p)
            clang++ ${CXX_FLAGS} ${CXX_FLAGS_GAME_DEBUG} -l ${LIBS} build/objects/*.o ${IMGUI_OBJECT_FILES_DIRECTORY}/*.o ${BANA_OBJECT_FILES_DIRECTORY}/*.o build/objects/*.res -o build/${EXE_NAME}
        elif [ "$MODE" = "release" ]; then
            files_in_flight=0
            for file in ${CXX_FILES[*]}; do
                echo $file
                clang++ ${CXX_FLAGS} ${CXX_FLAGS_GAME_RELEASE} -I ${INCLUDE} ${file} -c -o build/objects/$(basename ${file}).o &
                (( ++files_in_flight ));
                if (( $files_in_flight == $THREADS )); then
                    echo "Waiting for jobs to finish"
                    wait $(jobs -p)
                    files_in_flight=0
                fi
            done

            wait $(jobs -p)
            clang++ ${CXX_FLAGS} ${CXX_FLAGS_GAME_RELEASE} -l ${LIBS} ${BANA_OBJECT_FILES_DIRECTORY}/*.o build/objects/*.o -o build/${EXE_NAME}
        fi
        type ./thirdparty/tools/ctime.exe && ./thirdparty/tools/ctime.exe -end ./build/timings.ctm
    else
        if [ "$MODE" = "debug" ]; then
            files_in_flight=0
            for file in ${CXX_FILES[*]}; do
                echo $file
                clang++ ${CXX_FLAGS} ${CXX_FLAGS_GAME_DEBUG} -I ${INCLUDE} ${file} -c -o build/objects/$(basename ${file}).o &
                (( ++files_in_flight ));
                if (( $files_in_flight == $THREADS )); then
                    echo "Waiting for jobs to finish"
                    wait $(jobs -p)
                    files_in_flight=0
                fi
            done

                    wait $(jobs -p)
            clang++ ${CXX_FLAGS} ${CXX_FLAGS_GAME_DEBUG} -l ${LIBS} build/objects/*.o ${IMGUI_OBJECT_FILES_DIRECTORY}/*.o ${BANA_OBJECT_FILES_DIRECTORY}/*.o -o build/${EXE_NAME}
        elif [ "$MODE" = "release" ]; then
            files_in_flight=0
            for file in ${CXX_FILES[*]}; do
                echo $file
                clang++ ${CXX_FLAGS} ${CXX_FLAGS_GAME_RELEASE} -I ${INCLUDE} ${file} -c -o build/objects/$(basename ${file}).o &
                (( ++files_in_flight ));
                if (( $files_in_flight == $THREADS )); then
                    echo "Waiting for jobs to finish"
                    wait $(jobs -p)
                    files_in_flight=0
                fi
            done

            wait $(jobs -p)
            clang++ ${CXX_FLAGS} ${CXX_FLAGS_GAME_RELEASE} -l ${LIBS} ${BANA_OBJECT_FILES_DIRECTORY}/*.o build/objects/*.o -o build/${EXE_NAME}
        fi
    fi
    exit 0
fi

if [ "${1}" = "shader" ]; then
    glslc shaders/main.frag -o build/frag.spv
    glslc shaders/main.vert -o build/vert.spv
    exit 0
fi

if [ "${1}" = "imgui" ]; then
    rm -f ${IMGUI_OBJECT_FILES_DIRECTORY}/*.o
    if [ "$OS" = "linux" ]; then
        for file in ${IMGUI_LINUX_CXX_FILES[*]}; do
            echo $file
            clang++ ${file} ${CXX_FLAGS} ${CXX_FLAGS_IMGUI} -I ${INCLUDE} -c -o ${IMGUI_OBJECT_FILES_DIRECTORY}/$(basename ${file}).o &
        done;

        wait $(jobs -p)
        exit 0
    elif [ "$OS" = "win32" ]; then
        for file in ${IMGUI_CXX_FILES[*]}; do
            echo $file
            clang++ ${file} ${CXX_FLAGS} ${CXX_FLAGS_IMGUI} -I ${INCLUDE} -c -o ${IMGUI_OBJECT_FILES_DIRECTORY}/$(basename ${file}).o &
        done;

        wait $(jobs -p)
        exit 0
    fi
fi

if [ "${1}" = "bana" ]; then
    build_bana
    exit 0
fi

echo Nothing to do
