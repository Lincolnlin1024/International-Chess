/****************************************************************************
** Meta object code from reading C++ file 'chessman.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.4.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../chessman.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'chessman.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.4.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
namespace {
struct qt_meta_stringdata_ChessMan_t {
    uint offsetsAndSizes[22];
    char stringdata0[9];
    char stringdata1[10];
    char stringdata2[5];
    char stringdata3[6];
    char stringdata4[7];
    char stringdata5[7];
    char stringdata6[5];
    char stringdata7[5];
    char stringdata8[10];
    char stringdata9[6];
    char stringdata10[6];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_ChessMan_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_ChessMan_t qt_meta_stringdata_ChessMan = {
    {
        QT_MOC_LITERAL(0, 8),  // "ChessMan"
        QT_MOC_LITERAL(9, 9),  // "CHESSTYPE"
        QT_MOC_LITERAL(19, 4),  // "KING"
        QT_MOC_LITERAL(24, 5),  // "QUEEN"
        QT_MOC_LITERAL(30, 6),  // "BISHOP"
        QT_MOC_LITERAL(37, 6),  // "KNIGHT"
        QT_MOC_LITERAL(44, 4),  // "ROOK"
        QT_MOC_LITERAL(49, 4),  // "PAWN"
        QT_MOC_LITERAL(54, 9),  // "CHESSTEAM"
        QT_MOC_LITERAL(64, 5),  // "WHITE"
        QT_MOC_LITERAL(70, 5)   // "BLACK"
    },
    "ChessMan",
    "CHESSTYPE",
    "KING",
    "QUEEN",
    "BISHOP",
    "KNIGHT",
    "ROOK",
    "PAWN",
    "CHESSTEAM",
    "WHITE",
    "BLACK"
};
#undef QT_MOC_LITERAL
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_ChessMan[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       2,   14, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // enums: name, alias, flags, count, data
       1,    1, 0x0,    6,   24,
       8,    8, 0x0,    2,   36,

 // enum data: key, value
       2, uint(ChessMan::KING),
       3, uint(ChessMan::QUEEN),
       4, uint(ChessMan::BISHOP),
       5, uint(ChessMan::KNIGHT),
       6, uint(ChessMan::ROOK),
       7, uint(ChessMan::PAWN),
       9, uint(ChessMan::WHITE),
      10, uint(ChessMan::BLACK),

       0        // eod
};

Q_CONSTINIT const QMetaObject ChessMan::staticMetaObject = { {
    QMetaObject::SuperData::link<QLabel::staticMetaObject>(),
    qt_meta_stringdata_ChessMan.offsetsAndSizes,
    qt_meta_data_ChessMan,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_ChessMan_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<ChessMan, std::true_type>
    >,
    nullptr
} };

void ChessMan::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    (void)_o;
    (void)_id;
    (void)_c;
    (void)_a;
}

const QMetaObject *ChessMan::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ChessMan::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ChessMan.stringdata0))
        return static_cast<void*>(this);
    return QLabel::qt_metacast(_clname);
}

int ChessMan::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QLabel::qt_metacall(_c, _id, _a);
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
