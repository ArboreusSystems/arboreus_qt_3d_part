/* -------------------------------------------------------------------
 *  @doc
 *  @notice Template file apch.h
 *
 *  @copyright Arboreus (http://arboreus.systems)
 *  @author Alexandr Kirilov (http://alexandr.kirilov.me)
 *  @created 23/11/2026 at 19:04:36
 * */// --------------------------------------------------------------

#ifndef ALIBERLANGINTERFACE_H_PCH
#define ALIBERLANGINTERFACE_H_PCH

#if defined __cplusplus

#pragma once

// System includes
#include <QObject>
#include <QTcpSocket>
#include <QHostInfo>
#include <QDeadlineTimer>
#include <QCryptographicHash>
#include <QRandomGenerator>
#include <QCoreApplication>
#include <QtEndian>
#include <atomic>
#include <algorithm>

// Erlang interface includes
extern "C" {
#include <ei.h>
}

// Application includes

#endif // defined __cplusplus

#endif // ALIBERLANGINTERFACE_H_PCH
